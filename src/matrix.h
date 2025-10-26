#ifndef RAY_TRACE_COORD_H
#define RAY_TRACE_COORD_H

#include "errors.h"
#include "type.h"

/** A row major matrix implementation.
 *
 * Since the matrix information is not known during compile-time, this matrix
 * implementation requires dynamic allocation and is initialized through
 * r_mat_alloc() or r_mat_set() functions. And, as such, this matrix must be
 * freed with the r_mat_free() function after it has been used. If not, a
 * memory leak will occur.
 */
typedef struct {
    I_MAT r;       ///< Row count of the matrix
    I_MAT c;       ///< Column count of the matrix
    RT_FLOAT* mat; ///< Pointer to matrix data
} r_matrix;

/// Create a new matrix with all values set to zero.
r_matrix r_mat_alloc(I_MAT row, I_MAT col);

/// Create a square identity matrix
r_matrix r_mat_id(I_MAT rowcol);

/// Create a new matrix from an array of floating point values.
RT_RES r_mat_set(I_MAT row, I_MAT col, const void* const data, size_t dat_size,
                 r_matrix* mat);

/// Set left matrix to have same data as the \b other.
RT_RES r_mat_memcpy(r_matrix* mat, const r_matrix other);

/// Copies the given matrix and returns a new one with identical data that
/// points to a new location.
r_matrix r_mat_clone(const r_matrix val);

/// Frees the given matrix.
void r_mat_free(r_matrix* mat);

/** Matrix multiplication
 *
 * @param left An LxN matrix
 * @param right An NxR matrix
 * @param res The result of the matrix multiplication. MUST already be
 * allocated and cannot be one of the inputs
 * @return 0 if successful, error code if not.
 */
RT_RES r_matmul(const r_matrix left, const r_matrix right, r_matrix* res);

/** *In place* matrix scalar multiplication
 *
 * @return 0 if successful, error code if not.
 */
RT_RES r_matscalmul(r_matrix* left, const RT_FLOAT right);

/** Matrix summation
 *
 * @param left An NxM matrix
 * @param right An NxM matrix
 * @param res The result of the summation
 * @returns 0 if successful, error code if not.
 */
RT_RES r_matsum(const r_matrix left, const r_matrix right, r_matrix* res);

/** Assigns a value to the matrix
 *
 * @return 0 if successful, error code if not.
 */
RT_RES r_mat_assign(r_matrix* mat, I_MAT r, I_MAT c, RT_FLOAT val);

/** Returns a value from the given matrix
 *
 * @param res The return value of the function if read is successful
 * @return 0 if successful, error code if not.
 */
RT_RES r_mat_get(const r_matrix mat, I_MAT r, I_MAT c, RT_FLOAT* res);

/// Prints a matrix to stdout. For debug purposes.
void r_mat_print(const r_matrix mat);

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

// Vector AND Matrix operations

/** Matrix and vector multiplication
 *
 * @param mat An Nx4 or Nx3 matrix
 * @param vec A vector
 * @param res Result of the operation which will be modified if the return value
 * is true
 * @return 0 if successful, error code if not.
 *
 * The matrix has to be Nx4 or Nx3, or `mat->c == 4 || mat->c == 3`
 */
RT_RES r_matvec_mul(const r_matrix mat, const vector3 vec, vector3* res);

/** Applies translation to the given 4x4 matrix.
 *
 * @param mat Matrix to be translated
 * @param vec Input vector (MUST BE NORMALIZED)
 * @return 0 if successful, error code if not.
 */
RT_RES vec_transmat(r_matrix* mat, const vector3 vec);

/** Applies rotation to the given matrix by angle \b rot around given vector \b
 * vec using the Rodrigues rotation formula.
 *
 * @param mat Matrix to be rotated
 * @param vec Input vector
 * @param rot Rotation angle
 */
RT_RES vec_rotmat(r_matrix* mat, const vector3 vec, RT_FLOAT rot);

#endif
