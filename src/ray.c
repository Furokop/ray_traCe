#include "ray.h"
#include "matrix.h"

ray ray_new(vector3 pos, vector3 path) {
    ray ret = {pos, path};
    return ret;
}

vector3 ray_dist(ray r, RT_FLOAT dist) {
    return vec_sum(r.pos, vec_mul(dist, r.path));
}
