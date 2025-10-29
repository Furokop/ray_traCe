#include "body.h"
#include <include/math.h>
#include <include/texture.h>
#include <include/util.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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
                *norm = vec_norm(vec_sub(p_col, C));
                return true;
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

    body_rep ret = {(void*)sph, sph_s, tex, &sphere_col, &free_generic_impl};

    return ret;
}

body_rep body_floor_new(RT_FLOAT y, ray_texture tex) {
    size_t flr_s = sizeof(body_floor);
    body_floor* flr;

    flr = (body_floor*)malloc(flr_s);
    flr->height = y;
    body_rep ret = {(void*)flr, flr_s, tex, &floor_col, &free_generic_impl};

    return ret;
}

bool floor_col(const body_rep* const body, const ray r, RT_FLOAT* dist,
               vector3* norm) {
    body_floor* flr = (body_floor*)body->body;
    RT_FLOAT y_diff;

    y_diff = r.pos.j - flr->height;
    // If ray is above the floor:
    if (y_diff >= 0.0) {
        // And if the ray points downwards
        if (y_diff * r.path.j < 0.0) {
            *dist = -y_diff / r.path.j;
            *norm = vec3(0.0, 1.0, 0.0);
            return true;
        } else {
            return false;
        }
    }
    // If ray is below the floor:
    else if (y_diff < 0.0) {
        // And if the ray points upwards
        if (y_diff * r.path.j < 0.0) {
            *dist = -y_diff / r.path.j;
            *norm = vec3(0.0, -1.0, 0.0);
            return true;
        } else {
            return false;
        }
    }
    return false;
}

void body_free(body_rep* body) {
    free(body->body);
    texture_free(&body->tex);
}

bool body_ray_col(const body_rep* const body, const ray ray_in,
                  RT_FLOAT* dist, ray* refl_ray, vector3* norm) {

    vector3 point;
    vector3 reflect;

    if (body->_col_impl(body, ray_in, dist, norm) == true) {
        // If we haven't hit the max reflection count yet:

        // Calculate reflected ray
        point = ray_dist(ray_in, *dist);
        reflect = vec_refl_diff(ray_in.path, *norm, body->tex.diffusivity);

        // Form new reflected ray
        *refl_ray = ray_new(point, reflect);

        // Use the new ray
        // This sort of has the function:
        // color = current_color * (1 - current_reflectivity) +
        // (current_reflectivity * recursive())

        return true;
    }
    // This only matters if there has been absolutely no collision
    return false;
}
