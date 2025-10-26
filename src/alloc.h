#ifndef RAY_TRACE_ALLOC_H
#define RAY_TRACE_ALLOC_H

#include "errors.h"
#include <stddef.h>

#define RTVEC_DEF_COUNT 10
#define RTVEC_DEF_EXP 1.25

// Vector type implementation
typedef struct {
    // data count
    size_t data_count;
    // size of data block
    size_t max_data_count;
    // pointer to data
    void* data;
    // sizeof(data_t)
    size_t data_size;
} rtvec;

/// Initialize new rtvec type.
rtvec rtvec_alloc_n(size_t n, size_t size);
/// Initialize new rtvec type.
rtvec rtvec_alloc(size_t size);
/// Reallocate vector and resize.
void rtvec_realloc(rtvec* vec, int n);
/// Push data (data is copied)
void rtvec_push(rtvec* vec, void* data);

/// Pop data.
///
/// This does not return the data but just eliminates it. (Actually, it
/// just decrements data count, new data will just overwrite the old one)
void rtvec_pop(rtvec* vec);

/// Return the pointer to the first member of the vector (basically vec.data)
void* rtvec_begin(const rtvec* const vec);

/// Return the pointer to the last member of the vector
void* rtvec_end(const rtvec* const vec);

/// Free the rtvec
void rtvec_free(rtvec* vec);

/// Set result parameter to pointer to the nth index
RT_RES rtvec_get(const rtvec* const vec, size_t n, void* ret);

/// Doubly linked list
typedef struct rtdll {
    struct rtdll* next; ///< Next item
    struct rtdll* prev; ///< Previous item
    void* data;         ///<  Ptr to data
    size_t data_size;   ///< sizeof(data)
} rtdll;

/// Delete nth element
RT_RES rtdll_del(rtdll* ll, size_t n);

/// Generate a new rtdll from just data
rtdll rtdll_new(void* data, size_t data_size);

/// Generate a new rtdll from a list of data.
rtdll rtdll_new_list(void* data, size_t data_size, size_t n);

/// Push one data
void rtdll_push(rtdll* ll, void* data, size_t data_size);

/// Push a list of items. Generates a new doubly linked list object per data.
void rtdll_push_list(rtdll* ll, void* data, size_t data_size, size_t n);

/// Iterator macro for doubly linked list
#define DLL_ITER(n, head) for (rtdll* n = head; n->next != NULL; n = n->next)

/// Iterator macro for doubly linked list, n is already declared
#define DLL_ITER_EX(n, head) for (n = head; n->next != NULL; n = n->next)

#endif
