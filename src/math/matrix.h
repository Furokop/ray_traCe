#ifndef RAY_TRACE_COORD_H
#define RAY_TRACE_COORD_H

#include <include/errors.h>
#include <include/util.h>

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

#endif
