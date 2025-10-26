#include "util.h"
#include <stdlib.h>

void no_free_func(void *impl) {
    return;
}

void free_generic_impl(void* ptr) {
    free(ptr);
}
