#ifndef DIE_WITH_ERROR_HPP
#define DIE_WITH_ERROR_HPP 1

#include <cerrno>   // errno.
#include <cstdio>   // fprintf(), perror().
#include <cstdlib>  // exit().

inline void DieWithError(char *errorMessage) {
    perror(errorMessage);
    fprintf(stderr, "%d\n", errno);
    exit(1);
}

#endif  // DIE_WITH_ERROR_HPP
