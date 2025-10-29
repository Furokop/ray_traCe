#ifndef RAY_TRACE_VECTOR_H
#define RAY_TRACE_VECTOR_H

#include <include/util.h>

/// A 3D vector.
typedef struct {
    RT_FLOAT i; ///< X axis
    RT_FLOAT j; ///< Y axis
    RT_FLOAT k; ///< Z axis
} vector3;

vector3 vec3(RT_FLOAT i, RT_FLOAT j, RT_FLOAT k);

vector3 vec_rand(RT_FLOAT min, RT_FLOAT max);

vector3 vec_zero();

/// Cross product between two vectors
vector3 vec_cross(const vector3 left, const vector3 right);

/// Dot product between two vectors
RT_FLOAT vec_dot(const vector3 left, const vector3 right);

/// Magnitude of a vector
RT_FLOAT vec_mag(const vector3 vec);
/// Normalized version of the vector
vector3 vec_norm(const vector3 vec);

vector3 vec_sum(vector3 left, vector3 right);
vector3 vec_sub(vector3 left, vector3 right);
vector3 vec_mul(RT_FLOAT val, vector3 vec);

/// Given a vector and a normal, calculate the reflection vector
vector3 vec_refl(vector3 in, vector3 norm);

/** Get the diffused reflection from a vector and a normal
 *
 * @param path Path the ray originally followed for the collision
 * @param norm Normal vector from the point ray hit
 * @param diff Diffusivity value, 0 means no diffusion, 1.0 means max diffusion
 * @return A random direction vector based on input parameters
 */
vector3 vec_refl_diff(const vector3 path, const vector3 norm, RT_FLOAT diff);

#endif
