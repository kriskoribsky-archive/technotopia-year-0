#define _POSIX_C_SOURCE 200809L // strdup
#include <assert.h>
#include <stdio.h>
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

#define CHECK_EMPTY(s)    \
    {                     \
        if ((*s) == '\0') \
        {                 \
            return NULL;  \
        }                 \
    }

#define PATTERN_BUFFER_SIZE 256
#define ERROR_BUFFER_SIZE 256
#define REGCOMP_SUCCESS 0

struct command *create_command(char *name, char *description, char *pattern, size_t nmatch)
{
    CHECK_NULL(name);
    CHECK_NULL(description);
    CHECK_EMPTY(name);
    CHECK_EMPTY(description);

    // prepare pattern
    // char parsed_pattern[PATTERN_BUFFER_SIZE];
    // sprintf(parsed_pattern, pattern == NULL ? "^%s$" : "%s", pattern == NULL ? name : pattern);
    pattern = pattern != NULL ? pattern : name;

    struct command *new;
    MALLOC(1, new);

    new->name = strdup(name);
    new->description = strdup(description);
    ASSERT(new->name != NULL);
    ASSERT(new->description != NULL);

    // precompile pattern
    int rc;
    if ((rc = regcomp(&new->preg, pattern, REG_EXTENDED | REG_ICASE)) != REGCOMP_SUCCESS)
    {
        char buffer[ERROR_BUFFER_SIZE];
        regerror(rc, &new->preg, buffer, sizeof(buffer));
        fprintf(stderr, "Error! Compilation of regular expression '%s' failed.\n", buffer);
        exit(EXIT_FAILURE);
    }

    new->nmatch = nmatch;
    MALLOC(nmatch, new->groups);
    for (size_t i = 0; i < nmatch; i++)
    {
        new->groups[i] = NULL;
    }

    return new;
}

struct command *destroy_command(struct command *command)
{
    CHECK_NULL(command);

    FREE(command->name);
    FREE(command->description);
    regfree(&command->preg);
    for (size_t i = 0; i < command->nmatch; i++)
    {
        FREE(command->groups[i]);
    }
    FREE(command->groups);
    FREE(command);

    return command;
}