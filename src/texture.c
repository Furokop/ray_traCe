#include "texture.h"
#include "type.h"
#include "util.h"
#include <assert.h>
#include <stdlib.h>

color color_new(COLOR_TYPE r, COLOR_TYPE g, COLOR_TYPE b) {
    color ret = {color_constrain(r), color_constrain(g), color_constrain(b)};
    return ret;
}

COLOR_TYPE color_constrain(COLOR_TYPE val) {
    COLOR_TYPE ret;
    if (val > COLOR_MAX)
        ret = COLOR_MAX;
    else if (val < COLOR_MIN)
        ret = COLOR_MIN;
    else
        ret = val;
    return ret;
}

color color_sum(color left, color right) {
    COLOR_TYPE r, g, b;
    r = color_constrain(left.r + right.r);
    g = color_constrain(left.g + right.g);
    b = color_constrain(left.b + right.b);

    color ret = {r, g, b};

    return ret;
}

color color_mul(RT_FLOAT val, color col) {
    COLOR_TYPE r, g, b;
    r = color_constrain(val * col.r);
    g = color_constrain(val * col.g);
    b = color_constrain(val * col.b);

    color ret = {r, g, b};

    return ret;
}

color color_white() {
    color col = {COLOR_MAX, COLOR_MAX, COLOR_MAX};
    return col;
}

color color_black() {
    color col = {COLOR_MIN, COLOR_MIN, COLOR_MIN};
    return col;
}

color texture_single_impl(const void* const impl, const ray r,
                          const vector3 norm) {
    ray_texture_single_color* impl_data = (ray_texture_single_color*)impl;
    return impl_data->col;
}

ray_texture texture_new_single_color(color col, RT_FLOAT transparency, RT_FLOAT diffusivity) {
    assert(transparency < 1.0 && transparency > 0.0);
    ray_texture_single_color* tex_impl =
        (ray_texture_single_color*)malloc(sizeof(ray_texture_single_color));
    tex_impl->col = col;
    ray_texture ret = {(void*)tex_impl, true, transparency, diffusivity,
                       &texture_single_impl, &free_generic_impl};
    return ret;
}

void texture_free(ray_texture* tex) {
    tex->impl_free(tex->impl);
}
