#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "errors.h"
#include "matrix.h"
#include "type.h"

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

vector3 vec3(RT_FLOAT i, RT_FLOAT j, RT_FLOAT k) {
    vector3 ret = {i, j, k};
    return ret;
}

vector3 vec_rand(RT_FLOAT min, RT_FLOAT max) {
    float i = min + (float)rand()/(float)(RAND_MAX/max);
    float j = min + (float)rand()/(float)(RAND_MAX/max);
    float k = min + (float)rand()/(float)(RAND_MAX/max);
    return vec3(i, j, k);
}

vector3 vec_zero() {
    vector3 ret = {0.0, 0.0, 0.0};
    return ret;
}

vector3 vec_cross(const vector3 left, const vector3 right) {
    float r_i, r_j, r_k;

    r_i = left.j * right.k - left.k * right.j;
    r_j = left.k * right.i - left.i * right.k;
    r_k = left.i * right.j - left.j * right.i;
    vector3 ret = {r_i, r_j, r_k};
    return ret;
}

RT_FLOAT vec_dot(const vector3 left, const vector3 right) {
    float r_i, r_j, r_k, ret;

    r_i = left.i * right.i;
    r_j = left.j * right.j;
    r_k = left.k * right.k;
    ret = r_i + r_j + r_k;
    return ret;
}

RT_FLOAT vec_mag(const vector3 vec) {
    return sqrt(vec_dot(vec, vec));
}

vector3 vec_norm(const vector3 vec) {
    RT_FLOAT mag = vec_mag(vec);
    vector3 ret = vec_mul(1.0 / mag, vec);
    return ret;
}

vector3 vec_sum(vector3 left, vector3 right) {
    vector3 ret = {left.i + right.i, left.j + right.j, left.k + right.k};
    return ret;
}

vector3 vec_sub(vector3 left, vector3 right) {
    vector3 ret = {left.i - right.i, left.j - right.j, left.k - right.k};
    return ret;
}

vector3 vec_mul(RT_FLOAT val, vector3 vec) {
    vector3 ret = {vec.i * val, vec.j * val, vec.k * val};
    return ret;
}

vector3 vec_refl(vector3 in, vector3 norm) {
    vector3 ret = vec_sum(in, vec_mul(-2.0, vec_mul(vec_dot(in, norm), norm)));
    return ret;
}

vector3 vec_refl_diff(const vector3 path, const vector3 norm, RT_FLOAT diff) {
    vector3 ret = vec_refl(path, norm);
    vector3 rand = vec_rand(0.0, diff);
    ret = vec_sum(ret, rand);
    return ret;
}

RT_RES r_matvec_mul(const r_matrix mat, const vector3 vec, vector3* res) {
    if (mat.c == 4 && mat.r == 4) {
        RT_FLOAT vecmat[4] = {vec.i, vec.j, vec.k, 1.0};
        r_matrix tmp_mat;
        RET_IF_ERR(r_mat_set(4, 1, &vecmat, sizeof(vecmat), &tmp_mat));
        r_matrix res_mat = r_mat_alloc(4, 4);

        RET_IF_ERR(r_matmul(mat, tmp_mat, &res_mat));
        r_mat_get(res_mat, 0, 0, &res->i);
        r_mat_get(res_mat, 1, 0, &res->j);
        r_mat_get(res_mat, 2, 0, &res->k);

        r_mat_free(&tmp_mat);
        r_mat_free(&res_mat);
    } else if (mat.c == 3 && mat.r == 3) {
        RT_FLOAT vecmat[3] = {vec.i, vec.j, vec.k};
        r_matrix tmp_mat;
        r_matrix res_mat = r_mat_alloc(4, 4);

        RET_IF_ERR(r_mat_set(3, 1, &vecmat, sizeof(vecmat), &tmp_mat));
        RET_IF_ERR(r_matmul(mat, tmp_mat, &res_mat));
        r_mat_get(res_mat, 0, 0, &res->i);
        r_mat_get(res_mat, 1, 0, &res->j);
        r_mat_get(res_mat, 2, 0, &res->k);

        r_mat_free(&tmp_mat);
        r_mat_free(&res_mat);
    } else {
        RETURN_ERR(INCOMPATIBLE_MATRIX);
    }
    RETURN_NOERROR;
}

RT_RES vec_transmat(r_matrix* mat, const vector3 vec) {
    if (mat->c != 4 || mat->r != 4) {
        RETURN_ERR(INCOMPATIBLE_MATRIX);
    }
    r_matrix tmp_mat = r_mat_id(4);
    r_mat_assign(&tmp_mat, 0, 3, vec.i);
    r_mat_assign(&tmp_mat, 1, 3, vec.j);
    r_mat_assign(&tmp_mat, 2, 3, vec.k);

    r_mat_free(&tmp_mat);
    RETURN_NOERROR;
}

RT_RES vec_rotmat(r_matrix* mat, const vector3 vec, RT_FLOAT rot) {
    r_matrix mat_rot;
    r_matrix mat_tmp = r_mat_alloc(4, 4);

    if (fabs(vec_mag(vec) - 1.0) > RTFCOMPVAL) {
        RETURN_ERR(INCOMPATIBLE_VECTOR);
    }
    RT_FLOAT i = vec.i;
    RT_FLOAT j = vec.j;
    RT_FLOAT k = vec.k;

    RT_FLOAT c = cos(rot);
    RT_FLOAT s = sin(rot);
    RT_FLOAT omc = 1.0f - c;

    RT_FLOAT rot_data[] = {c + i * i * omc,
                           k * s + i * j * omc,
                           -j * s + i * k * omc,
                           0,
                           -k * s + i * j * omc,
                           c + j * j * omc,
                           i * s + j * k * omc,
                           0,
                           j * s + i * k * omc,
                           -i * s + j * k * omc,
                           c + k * k * omc,
                           0,
                           0,
                           0,
                           0,
                           1};
    r_mat_set(4, 4, rot_data, sizeof(rot_data), &mat_rot);

    r_matmul(mat_rot, *mat, &mat_tmp);
    r_mat_memcpy(mat, mat_tmp);

    r_mat_free(&mat_rot);
    r_mat_free(&mat_tmp);

    RETURN_NOERROR;
}
