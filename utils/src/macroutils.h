#ifndef _MACROUTILS_H
#define _MACROUTILS_H

#include "stdbool.h"

// Macros for unit testing
// ================================================================================

/* silence variable not used in GCC */
#ifdef __GNUC__
#define VARIABLE_IS_NOT_USED __attribute__((unused))
#else
#define VARIABLE_IS_NOT_USED
#endif

/* useful when testing main() functions */
#ifdef TEST
#define MAIN __target_main
#else
#define MAIN main
int VARIABLE_IS_NOT_USED(__target_main);
#endif

/* for use when testing main() function  */
#define TEST_STDIO(i, e, o) TEST_ASSERT_TRUE(test_main_stdio(TARGET, JOIN(DIR_IN, i), JOIN(DIR_IN, e), JOIN(DIR_OUT, o)))

// Macros for development
// ================================================================================

/* common operations */
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define FOREVER for (;;) /* infinite loop */

#define SWAP(t, x, y) \
    {                 \
        t tmp = x;    \
        x = y;        \
        y = x;        \
    }

/* strings */
#define CLEAR_INPUT_BUFFER()                        \
    {                                               \
        int c;                                      \
        while ((c = getchar()) != '\n' && c != EOF) \
            ;                                       \
    }

#define STRINGIFY(x) #x

#define JOIN_HELPER(s1, s2) s1 s2
#define JOIN(s1, s2) JOIN_HELPER(s1, s2)

/* assertions */
#ifdef DEBUG
#include <assert.h>
#define ASSERT(expr) assert(expr)
#else
#define ASSERT(expr) ((void)0)
#endif

/* pointers, dynamic memory management */
#define CHECK_NULL(ptr)    \
    {                      \
        if ((ptr) == NULL) \
        {                  \
            return NULL;   \
        }                  \
    }

#define CHECK_NULL_FALSE(ptr) \
    {                         \
        if ((ptr) == NULL)    \
        {                     \
            return false;     \
        }                     \
    }

#define CHECK_NULL_VOID(ptr) \
    {                        \
        if ((ptr) == NULL)   \
        {                    \
            return;          \
        }                    \
    }

#define CHECK_EMPTY(s)    \
    {                     \
        if ((*s) == '\0') \
        {                 \
            return NULL;  \
        }                 \
    }

#define MALLOC(nmemb, ptr)                          \
    {                                               \
        (ptr) = malloc((nmemb) * (sizeof(*(ptr)))); \
        assert((ptr) != NULL);                      \
    }

#define CALLOC(nmemb, ptr)                         \
    {                                              \
        (ptr) = calloc((nmemb), (sizeof(*(ptr)))); \
        assert((ptr) != NULL);                     \
    }

#define FREE(ptr)     \
    {                 \
        free(ptr);    \
        (ptr) = NULL; \
    }

#endif // _MACROUTILS_H
