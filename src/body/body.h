#ifndef RAY_TRACE_BODY_H
#define RAY_TRACE_BODY_H

#include <stdbool.h>
#include <stddef.h>

// 3D rectangular prism body
#include <include/math.h>
#include <include/texture.h>
#include <include/util.h>

/// Max reflection count
#define MAX_REFL 100

typedef struct body_rep body_rep;

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
     * @param norm Normal vector to the surface which the ray hit
     */
    bool (*_col_impl)(
        const struct body_rep* const body, const ray r, RT_FLOAT* dist,
        vector3* norm); ///< Collision function implementation. DONT call.
    void (*impl_free)(void* impl); ///< Body free function
} body_rep;

/// Spherical body geometric data
typedef struct body_sphere {
    vector3 center; ///< Sphere center
    RT_FLOAT R;     ///< Sphere radius
} body_sphere;

/** Returns a spherical body that interacts with rays.
 *
 * @param center Center of the sphere
 * @param radius Radius of the sphere
 * @return The body_rep that corresponds to the sphere.
 */
body_rep body_sphere_new(vector3 center, RT_FLOAT radius, ray_texture tex);

bool sphere_col(const body_rep* const body, const ray r, RT_FLOAT* dist,
                vector3* norm);

typedef struct {
    RT_FLOAT height;
} body_floor;

body_rep body_floor_new(RT_FLOAT y, ray_texture tex);

bool floor_col(const body_rep* const body, const ray r, RT_FLOAT* dist,
               vector3* norm);

/**  Frees the body pointer
 *
 * @param body Body to be freed
 */
void body_free(body_rep* body);

/**  Calculates whether there is a collision with given ray to a given
 * body. If so, the point is returned to the \c res parameter.
 *
 * @param body The body that is currently being tested
 * @param r Ray
 * @param col_out Final returned color value, if return value is true
 * @param dist Distance to the first collision, for avoiding displaying further
 * objects before closer
 * @param refl_ray Pointer to reflection ray
 * @param norm Pointer to collision surface normal
 * @return Whether the ray collides with the object or not.
 */
bool body_ray_col(const body_rep* const body, const ray ray_in, RT_FLOAT* dist,
                  ray* refl_ray, vector3* norm);

#endif
