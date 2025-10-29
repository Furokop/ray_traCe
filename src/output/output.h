#ifndef RAY_TRACE_OUTPUT_H
#define RAY_TRACE_OUTPUT_H

#include <include/body.h>
#include <include/math.h>
#include <include/texture.h>
#include <include/util.h>

#include <stddef.h>

/** The display type that holds information about the camera and also about the
 * implementation to make use of the output
 *
 * Currently there is no direction support, we face (1, 0, 0)
 *
 */
typedef struct display {
    unsigned int d_w;    ///< Output width in pixels
    unsigned int d_h;    ///< Output height in pixels
    RT_FLOAT fov;        ///< Output FOV (vertical);
    vector3 pos;         ///< Display position
    color* color_buffer; ///< Color buffer (size is d_w * d_h)
    // Output implementation data
    void* output_impl; ///< Pointer to output handler type implementation
    void (*out)(const struct display* const); ///< Implementaiton function
    void (*free_impl)(void* ptr); ///< Free function for implementation
} display;

display display_init(int w, int h, RT_FLOAT fov, vector3 pos,
                     void* buffer_out_impl,
                     void (*out)(const struct display* const),
                     void (*free_impl)(void* ptr));

/// Frees the display
void display_free(display* disp);

/** Runs the ray tracing routine using the given bodies and display
 * and fills the color buffer with the result. This function does NOT write the
 * buffer.
 *
 * @see write_display() for outputting the result of the ray trace
 *
 * @param disp Display data
 * @param bodies Pointer to beginning of a list of bodies
 * @param body_count Amount of bodies in the given pointer
 */
void display_run_rays(const display* const disp, const body_rep** const bodies,
                      size_t body_count);

/// Writes the display data using the data provided by the \b output_impl data
void display_write(const display* const disp);

/** Run the given ray across the objects provided
 *
 * @param bodies Pointer to a list of pointers to bodies
 * @param body_count Length of the list above
 * @param r Ray to be ran against
 * @param ignore Set to NULL or 0, this is meant for internal use
 * @param refl_c pointer to reflection counter
 *
 * @returns The final color value to be displayed on the monitor
 */
color display_iterate_single_ray(const body_rep** const bodies,
                                 size_t body_count, ray r, body_rep* ignore,
                                 int refl_c);

typedef struct {
    char* disp_out;
    size_t disp_out_size;
} disp_ppm;

void ppm_color(color val, char* list);
void ppm_out(const display* const disp);
void ppm_free(void* impl);

#endif
