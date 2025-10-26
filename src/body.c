#include "body.h"
#include "matrix.h"
#include "output.h"
#include "ray.h"
#include "texture.h"
#include "type.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_REFL 10

static bool body_ray_impl(const body_rep** const bodies, size_t body_count,
                          const body_rep* const body, const ray r,
                          RT_FLOAT* dist, vector3* norm, color* col_out,
                          int* refl_c);

bool sphere_col(const body_rep* const body, const ray r, RT_FLOAT* dist,
                vector3* norm) {
    // First collision distance to ray origin
    RT_FLOAT d_col1, d_col2;
    // Second collision distance to ray origin
    // Point of the first collision
    vector3 p_col;

    body_sphere* sph = (body_sphere*)body->body;
    const vector3 D = r.path;
    const vector3 Q = r.pos;
    const vector3 C = sph->center;
    const RT_FLOAT R = sph->R;

    vector3 oc = vec_sub(C, Q);
    RT_FLOAT a = 1; // D.D which since D has magnitude 1, is 1

    RT_FLOAT b = -2 * vec_dot(D, oc);
    RT_FLOAT c = vec_dot(oc, oc) - R * R;
    RT_FLOAT disc = b * b - 4 * a * c;
    if (disc >= 0) {
        d_col1 = (-b - sqrt(disc)) / (2 * a);
        d_col2 = (-b + sqrt(disc)) / (2 * a);
        if (d_col1 < 0.0) {
            if (d_col2 < 0.0) {
                return false;
            } else {
                p_col = ray_dist(r, d_col2);
                *dist = d_col2;
            }
        }
        p_col = ray_dist(r, d_col1);
        *dist = d_col1;
        *norm = vec_norm(vec_sub(p_col, C));

        return true;
    }
    return false;
}

body_rep body_sphere_new(vector3 center, RT_FLOAT radius, ray_texture tex) {
    size_t sph_s = sizeof(body_sphere);
    body_sphere* sph;

    sph = (body_sphere*)malloc(sph_s);
    sph->center = center;
    sph->R = radius;

    body_rep ret = {(void*)sph, sph_s, tex, &body_ray_col, &sphere_col};

    return ret;
}

void body_free(body_rep* body) {
    free(body->body);
    texture_free(&body->tex);
}

bool body_ray_col(const body_rep** const bodies, size_t body_count,
                  const body_rep* const body, const ray ray_in, color* col_out,
                  RT_FLOAT* dist) {
    vector3 vec = vec_zero();
    int refl_c = 0;
    return body_ray_impl(bodies, body_count, body, ray_in, dist, &vec, col_out,
                         &refl_c);
}

static bool body_ray_impl(const body_rep** const bodies, size_t body_count,
                          const body_rep* const body, const ray ray_in,
                          RT_FLOAT* dist, vector3* norm, color* col_out,
                          int* refl_c) {
    vector3 point;
    vector3 reflect;
    ray refl;
    color c = color_black();

    if (body->_col_impl(body, ray_in, dist, norm) == true) {
        // If we haven't hit the max reflection count yet:
        if (*refl_c < MAX_REFL) {
            // Calculate reflected ray
            point = ray_dist(ray_in, *dist);
            reflect = vec_refl(ray_in.path, *norm);

            // Form new reflected ray
            refl = ray_new(point, reflect);

            // Use the new ray
            // This sort of has the function:
            // color = current_color * (1 - current_reflectivity) +
            // (current_reflectivity * recursive())
            c = color_sum(
                color_mul(1.0 - body->tex.reflectivity,
                          body->tex.refl(body->tex.impl, ray_in, *norm)),
                color_mul(body->tex.reflectivity,
                          ray_run(bodies, body_count, refl, (void*)body)));
        }
        *refl_c += 1;

        *col_out = c;
        return true;
    }
    // This only matters if there has been absolutely no collision
    return false;
}
