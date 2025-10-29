#ifndef RAY_TRACE_VECMAT_H
#define RAY_TRACE_VECMAT_H

#include "matrix.h"
#include "vector.h"

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
