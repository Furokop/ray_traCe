#include <math.h>
#include <stdlib.h>

#include <include/util.h>

#include "vector.h"


vector3 vec3(RT_FLOAT i, RT_FLOAT j, RT_FLOAT k) {
    vector3 ret = {i, j, k};
    return ret;
}

vector3 vec_rand(RT_FLOAT min, RT_FLOAT max) {
    float i = min + (float)rand() / (float)(RAND_MAX / max);
    float j = min + (float)rand() / (float)(RAND_MAX / max);
    float k = min + (float)rand() / (float)(RAND_MAX / max);
    return vec3(i, j, k);
}

vector3 vec_zero() {
    vector3 ret = {0.0, 0.0, 0.0};
    return ret;
}

vector3 vec_cross(const vector3 left, const vector3 right) {
    float r_i, r_j, r_k;

    r_i = left.j * right.k - left.k * right.j;
    r_j = left.k * right.i - left.i * right.k;
    r_k = left.i * right.j - left.j * right.i;
    vector3 ret = {r_i, r_j, r_k};
    return ret;
}

RT_FLOAT vec_dot(const vector3 left, const vector3 right) {
    float r_i, r_j, r_k, ret;

    r_i = left.i * right.i;
    r_j = left.j * right.j;
    r_k = left.k * right.k;
    ret = r_i + r_j + r_k;
    return ret;
}

RT_FLOAT vec_mag(const vector3 vec) {
    return sqrt(vec_dot(vec, vec));
}

vector3 vec_norm(const vector3 vec) {
    RT_FLOAT mag = vec_mag(vec);
    vector3 ret = vec_mul(1.0 / mag, vec);
    return ret;
}

vector3 vec_sum(vector3 left, vector3 right) {
    vector3 ret = {left.i + right.i, left.j + right.j, left.k + right.k};
    return ret;
}

vector3 vec_sub(vector3 left, vector3 right) {
    vector3 ret = {left.i - right.i, left.j - right.j, left.k - right.k};
    return ret;
}

vector3 vec_mul(RT_FLOAT val, vector3 vec) {
    vector3 ret = {vec.i * val, vec.j * val, vec.k * val};
    return ret;
}

vector3 vec_refl(vector3 in, vector3 norm) {
    vector3 ret = vec_sum(in, vec_mul(-2.0, vec_mul(vec_dot(in, norm), norm)));
    return ret;
}

vector3 vec_refl_diff(const vector3 path, const vector3 norm, RT_FLOAT diff) {
    vector3 ret = vec_refl(path, norm);
    vector3 rand = vec_rand(0.0, diff);
    ret = vec_sum(ret, rand);
    return ret;
}
