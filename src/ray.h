#ifndef RAY_TRACE_RAY_H
#define RAY_TRACE_RAY_H

#include "matrix.h"
#include "type.h"
#include <stddef.h>

/// Ray that holds two vectors, one for position and one for direction.
typedef struct {
    vector3 pos;  ///< Cartesian coordinates
    vector3 path; ///< Unit vector
} ray;

/// Construct new ray
ray ray_new(vector3 pos, vector3 path);

vector3 ray_dist(ray r, RT_FLOAT dist);

#endif
