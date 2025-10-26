#include "errors.h"
#include <stdio.h>
#include <stdlib.h>

void print_callstack(FILE* fd){
#ifdef __GLIBC__ 
    #ifdef RT_DEBUG
        char **strings;
        size_t i, size;
        void *array[1024];

        size = backtrace(array, 1024);
        fprintf(fd, "-- BACKTRACE --\n");
        strings = backtrace_symbols(array, size);

        // i = 0 holds this function
        for (i = 0; i < size; i++)
            fprintf(fd, "%s\n", strings[i]);

        free(strings);
    #endif
#else
    return;
#endif
}
