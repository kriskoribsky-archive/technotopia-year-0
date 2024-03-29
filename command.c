#define _POSIX_C_SOURCE 200809L // strdup
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "helpers.h"
#include "command.h"

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
    pattern = pattern != NULL ? pattern : name;
    nmatch = nmatch < 1 ? 1 : nmatch; // at least one group for saving into history

    // precompile pattern
    regex_t preg;
    int rc;
    if ((rc = regcomp(&preg, pattern, REG_EXTENDED | REG_ICASE)) != REGCOMP_SUCCESS)
    {
        char buffer[ERROR_BUFFER_SIZE];
        regerror(rc, &preg, buffer, sizeof(buffer));
        fprintf(stderr, "Error! Compilation of regular expression '%s' failed.\n", buffer);
        exit(EXIT_FAILURE);
    }

    // allocate command
    struct command *new;
    MALLOC(1, new);
    MALLOC(nmatch, new->groups);

    new->name = strdup(name);
    new->description = strdup(description);
    new->preg = preg;
    new->nmatch = nmatch;
    for (size_t i = 0; i < nmatch; i++)
    {
        new->groups[i] = NULL;
    }

    assert(new->name != NULL);
    assert(new->description != NULL);
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