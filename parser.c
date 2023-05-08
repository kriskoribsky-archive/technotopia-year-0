#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <regex.h>

#include "parser.h"

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

#define COMMAND_FILE "./commands.txt"
#define COMMAND_MAX_LEN 256
#define REG_MATCH 0
#define ERROR_BUFFER_SIZE 256

/**
 * Trim whitespace before and after string.
 * This function does not allocate any data
 * to heap. It operates on existing memory.
 *
 * @param s string to be trimmed
 * @return pointer to the part of trimmed s
 */
char *trim(char *s);

struct parser *create_parser()
{
    struct parser *new;
    MALLOC(1, new);

    new->history = NULL;
    new->commands = NULL;

    // load list of available commands
    FILE *fp = fopen(COMMAND_FILE, "r");
    ASSERT(fp != NULL);

    // prepare file format
    char format[64];
    sprintf(format, " '%%%d[^']' '%%%d[^']' %%d '%%%d[^']'", COMMAND_MAX_LEN - 1, COMMAND_MAX_LEN - 1, COMMAND_MAX_LEN - 1);

    // prepare buffers
    size_t nmatch;
    char name[COMMAND_MAX_LEN], description[COMMAND_MAX_LEN], pattern[COMMAND_MAX_LEN];

    // load commands
    while (fscanf(fp, format, name, pattern, &nmatch, description) == 4)
    {
        struct container *c = create_container(new->commands, COMMAND, create_command(name, description, pattern, nmatch));
        new->commands = new->commands == NULL ? c : new->commands;
    }
    ASSERT(new->commands != NULL);

    fclose(fp);
    return new;
}

struct parser *destroy_parser(struct parser *parser)
{
    CHECK_NULL(parser);

    parser->history = destroy_containers(parser->history);
    parser->commands = destroy_containers(parser->commands);
    FREE(parser);

    return NULL;
}

struct command *parse_input(struct parser *parser, char *input)
{
    CHECK_NULL(parser);
    CHECK_NULL(input);

    input = trim(input);

    // list trough available commands
    for (struct container *current = parser->commands; current != NULL; current = current->next)
    {
        struct command *cmd = current->command;

        int rc;
        regmatch_t groups[cmd->nmatch];
        if ((rc = regexec(&cmd->preg, input, cmd->nmatch, groups, 0)) == REG_MATCH)
        {
            // save matched groups
            for (size_t i = 0; i < cmd->nmatch; i++)
            {
                ASSERT(groups[i].rm_so != -1);
                cmd->groups[i] = strndup(input + groups[i].rm_so, (size_t)(groups[i].rm_eo - groups[i].rm_so));
            }

            // TODO check if adding to history is apropriate here or in the caller
            return cmd;
        }
        // handle error
        else if (rc != REG_NOMATCH)
        {
            char buffer[ERROR_BUFFER_SIZE];
            regerror(rc, &cmd->preg, buffer, sizeof(buffer));
            fprintf(stderr, "Error matching regular expression: '%s'\n", buffer);
            exit(EXIT_FAILURE);
        }
    }

    return NULL;
}

char *trim(char *s)
{
    CHECK_NULL(s);

    // ltrim
    while (isspace(*s))
        s++;

    // rtrim
    char *end = s + strlen(s);
    while (isspace(*--end))
        ;
    *(end + 1) = '\0';

    return s;
}