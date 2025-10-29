#include <include/output.h>
#include <include/body.h>
#include <include/math.h>
#include <include/texture.h>
#include <include/util.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

display display_init(int w, int h, RT_FLOAT fov, vector3 pos,
                     void* buffer_out_impl,
                     void (*out)(const struct display* const),
                     void (*free_impl)(void*)) {
    color* buf = malloc(sizeof(color) * w * h);
    display ret = {w, h, fov, pos, buf, buffer_out_impl, out, free_impl};
    return ret;
}

void display_free(display* disp) {
    free(disp->color_buffer);
    disp->free_impl(disp->output_impl);
}

void display_run_rays(const display* const disp, const body_rep** const bodies,
                      size_t body_count) {
    color c;
    ray r;
    RT_FLOAT ratio = (RT_FLOAT)disp->d_w /
                     (RT_FLOAT)disp->d_h; // Width to height ratio for display
    RT_FLOAT z = 1.0; // Our pretend distance to the "display"
    RT_FLOAT fov_rad = disp->fov * M_PI / 180.0;

    // Fake physical Y distance between between top and bottom ends of the
    // display and the center
    RT_FLOAT disp_y = tan(fov_rad / 2.0) * z;
    // Fake physical X distance between between right and right ends of the
    // display and the center
    RT_FLOAT disp_x = disp_y * ratio;

    // Heigth iteration
    for (int i = 0; i < disp->d_h; i++) {
        // Width iteration
        for (int j = 0; j < disp->d_w; j++) {
            size_t index = i * disp->d_w + j;
            // Construct fake coordinate to determine the path of the ray
            vector3 path = vec_norm(vec3(
                (2.0 * (j + 0.5) / (RT_FLOAT)disp->d_w - 1.0) * disp_x,
                (1.0 - 2.0 * (i + 0.5) / (RT_FLOAT)disp->d_h) * disp_y, z));
            r = ray_new(disp->pos, path);
            c = display_iterate_single_ray(bodies, body_count, r, NULL,
                                           MAX_REFL);
            disp->color_buffer[index] = c;
        }
    }
}

void display_write(const display* const disp) {
    disp->out(disp);
}

color display_iterate_single_ray(const body_rep** const bodies,
                                 size_t body_count, ray r, body_rep* ignore,
                                 int refl_c) {
    body_rep const* ref;
    color ret, c_tmp;
    RT_FLOAT z, z_min;
    ray refl;
    vector3 norm;
    bool first_col;

    first_col = true;
    z = 0.0;
    z_min = 0.0;

    // Background color as well
    ret = color_new(0.71, 0.784, 0.798);
    // Iterate through each body
    for (size_t i = 0; i < body_count; i++) {
        ref = bodies[i];
        if (ref == ignore) {
            continue;
        }
        // If we collide
        if (body_ray_col(ref, r, &z, &refl, &norm)) {
            if (refl_c != 0) {
                c_tmp =
                    color_sum(color_mul(1.0 - ref->tex.reflectivity,
                                        ref->tex.refl(ref->tex.impl, r, norm)),
                              color_mul(ref->tex.reflectivity,
                                        display_iterate_single_ray(
                                            bodies, body_count, refl,
                                            (body_rep*)ref, refl_c - 1)));
            } else {
                c_tmp = color_mul(1.0 - ref->tex.reflectivity,
                                  ref->tex.refl(ref->tex.impl, r, norm));
            }
            // We only draw the closest object
            if (first_col || z < z_min) {
                first_col = false;
                z_min = z;
                ret = c_tmp;
            }
        }
    }
    return ret;
}

void ppm_color(color val, char* list) {
    unsigned char r, g, b;
    r = (unsigned char)((val.r) * 255.999);
    g = (unsigned char)((val.g) * 255.999);
    b = (unsigned char)((val.b) * 255.999);
    sprintf(list, "%u %u %u", r, g, b);
}

void ppm_out(const display* const disp) {
    char color_char[12];
    color c;
    disp_ppm* impl = (disp_ppm*)disp->output_impl;
    FILE* f = fopen(impl->disp_out, "w");
    fprintf(f, "P3\n");
    fprintf(f, "%u %u\n", disp->d_w, disp->d_h);
    fprintf(f, "255\n");
    // Height
    for (int i = 0; i < disp->d_h; i++) {
        // Width
        for (int j = 0; j < disp->d_w; j++) {
            size_t index = i * disp->d_w + j;
            c = disp->color_buffer[index];
            ppm_color(c, color_char);
            fprintf(f, "%s", color_char);
            if (j != disp->d_w - 1)
                fprintf(f, " ");
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

void ppm_free(void* impl) {
    disp_ppm* disp = (disp_ppm*)impl;
    free(disp->disp_out);
}
