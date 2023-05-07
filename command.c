#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "command.h"

#ifdef DEBUG
#include <assert.h>
#define ASSERT(expr) assert(expr)
#else
#define ASSERT(expr) ((void)0)
#endif

#define MALLOC(nmemb, ptr)                          \
    {                                               \
        (ptr) = malloc((nmemb) * (sizeof(*(ptr)))); \
        assert((ptr) != NULL);                      \
    }

#define FREE(ptr)     \
    {                 \
        free(ptr);    \
        (ptr) = NULL; \
    }

#define CHECK_NULL(ptr)    \
    {                      \
        if ((ptr) == NULL) \
        {                  \
            return NULL;   \
        }                  \
    }

struct command *create_command(char *name, char *description, char *pattern, size_t nmatch)
{
    CHECK_NULL(name);
    CHECK_NULL(description);

    struct command *new;
    MALLOC(1, new);

    new->name = strdup(name);
    new->description = strdup(description);
    ASSERT(new->name != NULL &&new->description != NULL);

    // precompile pattern
    int result = regcomp(&new->preg, pattern, REG_EXTENDED | REG_ICASE | REG_NOSUB);
    /*
        REG_EXTENDED    Use POSIX Extended Regular Expression syntax
        REG_ICASE       Pattern buffer will be case insensitive
        REG_NOSUB       Do not report position of group matches
    */
    ASSERT(result == 0);
    if (result != 0)
    {
        return NULL;
    }

    new->nmatch = nmatch;
    new->groups = NULL;

    return new;
}

struct command *destroy_command(struct command *command)
{
    CHECK_NULL(command);

    FREE(command->name);
    FREE(command->description);

    for (int i = 0; i < command->nmatch; i++)
    {
        FREE(command->groups[i]);
    }

    FREE(command);
    return NULL;
}