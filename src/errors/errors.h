#ifndef RAY_TRACE_ERRORS_H
#define RAY_TRACE_ERRORS_H

#include <execinfo.h>
#include <stddef.h>
#include <stdio.h>

#define STR(x) STR2(x)
#define STR2(x) #x
#define __LINESTR__ STR(__LINE__)

/** Prints the callstack if the \b RT_DEBUG is defined during compilation
 *
 * @param fd File descriptor (likely stderr) to print to
 *
 * Note that with debug builds RT_DEBUG will be set automatically.
 */
void print_callstack(FILE* fd);

/// Result type enumerator.
///
/// The RET_IF_ERR() macro should be used to return if any error is encountered.
typedef enum {
    ALL_GOOD,
    OUT_OF_BOUNDS,
    INCOMPATIBLE_MATRIX,
    INCOMPATIBLE_VECTOR,
} RT_RES_TYPE;

/// Result type struct that also holds information about where it is declared.
typedef struct {
    RT_RES_TYPE type;
    char* msg;
} RT_RES;

/** If there is an error, it returns from the function.
 *
 * Naturally, the way this works requires a chain of RT_RES return values with
 * an error handler on top. */
#define RET_IF_ERR(x)                                                          \
    if (x.type != ALL_GOOD)                                                    \
        return x;

/// Returns an RT_RES type with information about its source.
#define GEN_ERR(type)                                                          \
    {type, "\n---- ERROR ----\nError type " #type                              \
           ":\nError originating from: " __FILE__ ":" STR(__LINE__) "\n"}

/// Return macro for the given error
#define RETURN_ERR(type)                                                       \
    {                                                                          \
        RT_RES RTRESRET = GEN_ERR(type);                                       \
        return RTRESRET;                                                       \
    }

/// Describes a successful return.
#define RETURN_NOERROR                                                         \
    RT_RES RTRESRET = GEN_ERR(ALL_GOOD);                                       \
    return RTRESRET;

/// Print the error if its not "ALL_GOOD"
#define PRINT_ERR(err)                                                         \
    if (err.type != ALL_GOOD) {                                                \
        fprintf(stderr, "%s", err.msg);                                        \
        fprintf(stderr,                                                        \
                "Error printed from:     " __FILE__ ":" STR(__LINE__) "\n");   \
        print_callstack(stderr);                                               \
        fprintf(stderr, "---- ERROR ----\n\n");                                \
    }

/// fprintf wrapper if RT_DEBUG=1, else it is just nothing
#ifdef RT_DEBUG
#define DEBUG_PRINT(fd, msg) fprintf(fd, "%s\n", msg);
#else
#define DEBUG_PRINT(fd, msg)
#endif

#endif
