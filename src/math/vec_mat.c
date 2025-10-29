#include "vec_mat.h"
#include <include/errors.h>

#include <math.h>

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
