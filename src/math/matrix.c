#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <include/errors.h>
#include "matrix.h"
#include <include/util.h>

r_matrix r_mat_alloc(I_MAT row, I_MAT col) {
    I_MAT n = row * col;
    RT_FLOAT* mat = (RT_FLOAT*)calloc(n, sizeof(RT_FLOAT));
    r_matrix ret = {row, col, mat};
    return ret;
}

r_matrix r_mat_id(I_MAT rowcol) {
    r_matrix ret = r_mat_alloc(rowcol, rowcol);
    // Create the identity matrix
    for (int i = 0; i < rowcol; i++) {
        r_mat_assign(&ret, i, i, 1.0);
    }
    return ret;
}

RT_RES r_mat_set(I_MAT row, I_MAT col, const void* const data, size_t dat_size,
                 r_matrix* mat) {
    // dat_size is in bytes. size of list given
    if (dat_size != row * col * sizeof(RT_FLOAT)) {
        RETURN_ERR(OUT_OF_BOUNDS);
    }

    *mat = r_mat_alloc(row, col);
    memcpy(mat->mat, data, dat_size);

    RETURN_NOERROR;
}

RT_RES r_mat_memcpy(r_matrix* mat, const r_matrix other) {
    if (mat->c != other.c || mat->r != other.r) {
        RETURN_ERR(INCOMPATIBLE_MATRIX);
    }

    memcpy(mat->mat, other.mat, other.c * other.r * sizeof(RT_FLOAT));
    RETURN_NOERROR;
}

r_matrix r_mat_clone(const r_matrix val) {
    r_matrix ret = {val.r, val.c, NULL};
    memcpy(&ret.mat, val.mat, sizeof(RT_FLOAT) * val.r * val.c);
    return ret;
}

void r_mat_free(r_matrix* mat) {
    free(mat->mat);
}

RT_RES r_matmul(const r_matrix left, const r_matrix right, r_matrix* res) {
    // Return matrix MUST NOT be one of the inputs
    if (res->mat == left.mat || res->mat == right.mat) {
        RETURN_ERR(INCOMPATIBLE_MATRIX);
    }

    I_MAT lr = left.r;
    I_MAT lc = left.c;
    I_MAT rr = right.r;
    I_MAT rc = right.c;

    if (lc != rr) {
        RETURN_ERR(INCOMPATIBLE_MATRIX);
    }

    RT_FLOAT l, r, n;
    // Row index
    for (I_MAT i = 0; i < lr; i++) {
        // Column index
        for (I_MAT j = 0; j < rc; j++) {
            n = 0.0;
            // Iterate
            for (I_MAT k = 0; k < lc; k++) {
                RET_IF_ERR(r_mat_get(left, i, k, &l));
                RET_IF_ERR(r_mat_get(right, k, j, &r));
                n += l * r;
            }
            r_mat_assign(res, i, j, n);
        }
    }
    RETURN_NOERROR;
}

RT_RES r_matscalmul(r_matrix* left, const RT_FLOAT right) {
    RT_FLOAT n, v;

    // Row
    for (I_MAT i = 0; i < left->r; i++) {
        // Column
        for (I_MAT j = 0; j < left->c; j++) {
            RET_IF_ERR(r_mat_get(*left, i, j, &v));
            n = v * right;
            r_mat_assign(left, i, j, n);
        }
    }
    RETURN_NOERROR;
}

RT_RES r_matsum(const r_matrix left, const r_matrix right, r_matrix* res) {
    RT_FLOAT l, r;
    if (left.c != right.c || left.r != right.r || left.c != res->c ||
        left.r != res->r) {
        RETURN_ERR(INCOMPATIBLE_MATRIX);
    }
    // Row
    for (I_MAT i = 0; i < left.r; i++) {
        // Column
        for (I_MAT j = 0; j < left.c; j++) {
            RET_IF_ERR(r_mat_get(left, i, j, &l));
            RET_IF_ERR(r_mat_get(right, i, j, &r));
            RET_IF_ERR(r_mat_assign(res, i, j, l + r));
        }
    }
    RETURN_NOERROR;
}

RT_RES r_mat_assign(r_matrix* mat, I_MAT r, I_MAT c, RT_FLOAT val) {
    if (r >= mat->r || c >= mat->c) {
        RETURN_ERR(OUT_OF_BOUNDS);
    }

    I_MAT i;

    i = mat->c * r + c;

    mat->mat[i] = val;

    RETURN_NOERROR;
}

RT_RES r_mat_get(const r_matrix mat, I_MAT r, I_MAT c, RT_FLOAT* res) {
    if (r >= mat.r || c >= mat.c) {
        RETURN_ERR(OUT_OF_BOUNDS);
    }

    I_MAT i;

    i = mat.c * r + c;
    *res = mat.mat[i];
    RETURN_NOERROR;
}

void r_mat_print(const r_matrix mat) {
    RT_FLOAT val;
    printf("[");
    for (int i = 0; i < mat.r; i++) {
        if (i != 0)
            printf(" ");
        for (int k = 0; k < mat.c; k++) {
            r_mat_get(mat, i, k, &val);
            printf("%f", val);
            if (k != mat.c - 1)
                printf(", ");
        }
        if (i != mat.r - 1)
            printf("\n");
    }
    printf("]");
}
