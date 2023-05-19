#ifndef _HELPERS_H
#define _HELPERS_H

#include <stddef.h>



// Helper macros
// ================================================================================

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

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



// Helper functions
// ================================================================================

/**
 * Trims leading and trailing whitespace from a string.
 *
 * @param out Pointer to the output buffer where the trimmed string will be stored.
 * @param len Size of the output buffer.
 * @param s Pointer to the input string to be trimmed.
 * @return The size of the trimmed string stored in the output buffer.
 *
 * @remarks The output buffer must have a size of at least 'len' to ensure safe storage of the trimmed string.
 *          If the output buffer is too small to hold the entire trimmed string, the string will be truncated
 *          to fit within the buffer, and the returned size will reflect the actual length of the trimmed string.
 */
size_t trim(char *out, size_t len, const char *s);

#endif