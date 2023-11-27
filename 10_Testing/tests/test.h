#pragma once
#include <stdio.h>
#include "buf.h"

#  define C_RED(s)     "\033[31;1m" s "\033[0m"
#  define C_GREEN(s)   "\033[32;1m" s "\033[0m"

#define TEST(s, x) \
do { \
        if (x) { \
            printf(C_GREEN("PASS") " %s\n", s); \
        } else { \
            printf(C_RED("FAIL") " %s\n", s); \
            return 1; \
        } \
    } while (0)



