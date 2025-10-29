#ifndef RAY_TRACE_TEXTURE_H
#define RAY_TRACE_TEXTURE_H

#include <include/math.h>
#include <include/util.h>

#include <stdbool.h>

#define COLOR_MAX 1.0
#define COLOR_MIN 0.0
#define COLOR_TYPE float

/// RGBA Color information
typedef struct {
    COLOR_TYPE r; ///< Red
    COLOR_TYPE g; ///< Green
    COLOR_TYPE b; ///< Blue
} color;

color color_new(COLOR_TYPE r, COLOR_TYPE g, COLOR_TYPE b);

/// Constrain color value between min and max
COLOR_TYPE color_constrain(COLOR_TYPE);

/// Sums two colors, if any of the resultant color values are above \b
/// COLOR_MAX then it is set to that, same logic for \b COLOR_MIN
color color_sum(color left, color right);

/// Multiplies color, if any of the resultant color values are above \b
/// COLOR_MAX then it is set to that, same logic for \b COLOR_MIN
color color_mul(RT_FLOAT val, color col);

/// White color (all \b COLOR_MAX)
color color_white();

/// Black color (all \b COLOR_MIN)
color color_black();

/** Struct that describes how to acquire color and stores the colors in a
 * vector if needed.
 *
 * A function pointer matching the \b refl signature must be provided in
 * order to provide a way to access data. If a color is to be acquired
 * programmatically, then the vector could be left empty.
 *
 * @see rttex_free free function
 */
typedef struct ray_texture {
    void* impl;            ///< Texture implementation
    bool does_reflect;     ///< Does this texture reflect light?
    RT_FLOAT reflectivity; ///< If does_reflect == true, how much does this
                           ///< texture reflect? (MUST be between 0 and 1, 0 =
                           ///< Absorbs all light, 1 = reflects ALL)
    RT_FLOAT diffusivity;  ///< How much will the reflections diffuse? 0 = NONE,
                           ///< 1 = ALL
    /** Interaction function for the surface itself.
     *
     * @param impl Texture implementation
     * @param r Ray
     * @param norm Surface normal at the point of interaction
     *
     * @return Color value
     */
    color (*refl)(const void* const impl, const ray r, const vector3 norm);
    void (*impl_free)(
        void* impl); ///< Free the impl object (called from rttex_free())
} ray_texture;

/// Single color texture implementation
/// @see rttex_single_impl implementation function
typedef struct {
    color col;
} ray_texture_single_color;

color texture_single_impl(const void* const impl, const ray r,
                          const vector3 norm);

/// Initialize a new empty texture struct with the given color
ray_texture texture_new_single_color(color col, RT_FLOAT reflectivity,
                                     RT_FLOAT diffusivity);

/// Free the texture struct given.
void texture_free(ray_texture* tex);

#endif
