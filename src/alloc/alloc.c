#include "alloc.h"
#include <include/errors.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

rtvec rtvec_alloc_n(size_t n, size_t size) {
    void* data = malloc(n * size);
    rtvec ret = {0, n, data, size};
    return ret;
}

rtvec rtvec_alloc(size_t size) {
    return rtvec_alloc_n(RTVEC_DEF_COUNT, size);
}

void rtvec_realloc(rtvec* vec, int n) {
    vec->max_data_count = n;
    void* ptr = realloc(vec->data, n * vec->data_size);
    vec->data = ptr;
}

void rtvec_push(rtvec* vec, void* data) {
    if (vec->max_data_count >= vec->data_count) {
        // If vector is max size, expand the vector
        int n = vec->max_data_count * RTVEC_DEF_EXP;
        rtvec_realloc(vec, n);
    }
    size_t byte_count = vec->data_size * vec->data_count;
    void* end = vec->data + byte_count;
    memcpy(end, data, vec->data_size);
    vec->data_count++;
}

void rtvec_pop(rtvec* vec) {
    // That's it
    vec->data_count--;
}

void* rtvec_begin(const rtvec* const vec) {
    return vec->data;
}

void* rtvec_end(const rtvec* const vec) {
    return vec->data + vec->data_count * vec->data_size;
}

void rtvec_free(rtvec* vec) {
    free(vec->data);
}

RT_RES rtvec_get(const rtvec* const vec, size_t n, void* ret) {
    if (n >= vec->data_count) {
        RETURN_ERR(OUT_OF_BOUNDS);
    }
    ret = vec->data + vec->data_count * vec->data_size;
    RETURN_NOERROR;
}

RT_RES rtdll_del(rtdll* ll, size_t n) {
    size_t _n = 0;
    DLL_ITER(w, ll) {
        _n++;
        // Then we are at the node that must go
        if (_n == n) {
            if (w->prev != NULL) {
                w->prev->next = w->next;
            }
            if (w->next != NULL) {
                w->next->prev = w->prev;
            }
            free(w);
            RETURN_NOERROR;
        }
    }
    RETURN_ERR(OUT_OF_BOUNDS)
}

rtdll rtdll_new(void* data, size_t data_size) {
    rtdll ret;
    ret.next = NULL;
    ret.next = NULL;
    ret.data = data;
    ret.data_size = data_size;
    return ret;
}

rtdll rtdll_new_list(void* data, size_t data_size, size_t n) {
    rtdll ret = rtdll_new(data, data_size);
    for (int i = 1; i < n; i++) {
        rtdll_push(&ret, data + i * data_size, data_size);
    }
    return ret;
}

void rtdll_push(rtdll* ll, void* data, size_t data_size) {
    rtdll* new = malloc(sizeof(*ll));
    new->data_size = data_size;
    new->data = data;

    // For iterating
    rtdll* n;
    DLL_ITER_EX(n, ll) {}
    new->prev = n;
    n->next = new;
}

void rtdll_push_list(rtdll* ll, void* data, size_t data_size, size_t n) {
    for (size_t i = 0; i < n; i++) {
        rtdll_push(ll, data + i * data_size, data_size);
    }
}
