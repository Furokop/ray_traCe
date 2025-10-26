#ifndef RAY_TRACE_BODY_H
#define RAY_TRACE_BODY_H

#include <stdbool.h>
#include <stddef.h>

// 3D rectangular prism body
#include "matrix.h"
#include "ray.h"
#include "texture.h"
#include "type.h"

typedef struct body_rep body_rep;

bool sphere_col(const body_rep* const body, const ray r, RT_FLOAT* dist,
                vector3* norm);

/// Rectangular prism body geometric data
typedef struct body_box {
    vector3 center; ///< Rectangle center
    vector3 point;  ///< Rectangle direction
    RT_FLOAT w_x;   ///< Width x
    RT_FLOAT w_y;   ///< Width y
    RT_FLOAT w_z;   ///< Width z
} body_box;

/// Spherical body geometric data
typedef struct body_sphere {
    vector3 center; ///< Sphere center
    RT_FLOAT R;     ///< Sphere radius
} body_sphere;

/** General type for pointing to bodies
 *
 * We use function pointers per each object type. I suppose supporting mesh
 * types and merely relying on them instead of resorting to abstractions like
 * this is better. However, for simplicity's sake, I will mainly be using simple
 * shapes and not meshes.
 *
 * Any object can be implemented using this struct as long as they provide all
 * the required methods that match the required function signature:
 * - dist_func: Returns the distance from a point to the object.
 * - ray_col: Returns whether there is a collision or not.
 * - ray_inter: Returns a new ray from the given interaction. The default
 * function is to reflect the ray however if so desired alternative behaviors
 * could also be implemented.
 *
 * Existing objects that are already implemented:
 * - Spheres: new_body_sphere()
 * - Rectangular Prism: new_body_box()
 */
typedef struct body_rep {
    void* body;       ///< Pointer to body
    size_t body_size; ///< Size of the body in bytes
    ray_texture tex;  ///< Texture information
    /** Collision function
     *
     * @param body Pointer to body for the function to use
     * @param r Ray
     * @param dist If return value is true, value at this pointer is set to
     * distance from ray origin to first collision.
     * @param col_out If returned true, returned color value from the ray.
     */
    bool (*col)(const struct body_rep** const bodies, size_t body_count,
                const struct body_rep* const body, const ray r, color* col_out,
                RT_FLOAT* dist);
    bool (*_col_impl)(
        const struct body_rep* const body, const ray r, RT_FLOAT* dist,
        vector3* norm); ///< Collision function implementation. DONT call.
} body_rep;

/** Returns a spherical body that interacts with rays.
 *
 * @param center Center of the sphere
 * @param radius Radius of the sphere
 * @return The body_rep that corresponds to the sphere.
 */
body_rep body_sphere_new(vector3 center, RT_FLOAT radius, ray_texture tex);

/**  Returns a rectangular prism that interacts with rays.
 *
 * @param center Center of prism
 * @param point The vector that the prism faces
 * @param width_x X direction
 * @param width_y Y direction
 * @param width_z Z direction
 * @return The body_rep that corresponds to the prism
 *
 * UNIMPLEMENTED!
 */
body_rep body_box_new(vector3 center, vector3 point, RT_FLOAT width_x,
                      RT_FLOAT width_y, RT_FLOAT width_z);

/**  Frees the body pointer
 *
 * @param body Body to be freed
 */
void body_free(body_rep* body);

/**  Calculates whether there is a collision with given ray to a given
 * body. If so, the point is returned to the \c res parameter.
 *
 * @param body Pointer to the body representation
 * @param col Ray
 * @param col_p Collision point is set to this, if the return value is true.
 * Else it is unchanged.
 * @param col_inter The interaction that results between the ray and the object.
 * This can be a reflection, refraction etc. return value is true. If there is
 * no collision it is unchanged.
 * @return Whether the ray collides with the object or not.
 */
bool body_ray_col(const body_rep** const bodies, size_t body_count,
                  const body_rep* const body, const ray r, color* col_out,
                  RT_FLOAT* dist);

#endif
