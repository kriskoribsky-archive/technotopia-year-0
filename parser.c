#define _POSIX_C_SOURCE 200809L // strdup
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "parser.h"

#ifdef DEBUG
#include <assert.h>
#define ASSERT(expr) assert(expr)
#else
#define ASSERT(expr) ((void)0)
#endif

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

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
    // predefine commands
    struct command *commands[] =
        {
            create_command("Inspect", "Description of the item in the backpack/room.", "^(PRESKUMAJ|INSPECT)( (.*))?$", 4),
            create_command("Take", "Take an item into the backpack.", "^(VEZMI|TAKE)( (.*))?$", 4),
            create_command("Put", "Place the item from the backpack.", "^(POLOZ|PUT)( (.*))?$", 4),
            create_command("Use", "Use item from backpack or room.", "^(POUZI|USE)( (.*))?$", 4),

            create_command("Look around", "Scan room for information.", "^(ROZHLIADNI SA|LOOK AROUND)$", 2),
            create_command("Inventory", "View the contents of the backpack.", "^(INVENTAR|INVENTORY|I)$", 2),
            create_command("Commands", "List of all game commands.", "^(PRIKAZY|POMOC|HELP|COMMANDS)$", 2),

            create_command("North", "Go north from your current position.", "^(SEVER|N|NORTH)$", 2),
            create_command("South", "Go south from your current position.", "^(JUH|S|SOUTH)$", 2),
            create_command("East", "Go east from your current position.", "^(VYCHOD|E|EAST)$", 2),
            create_command("West", "Go west from your current position.", "^(ZAPAD|W|WEST)$", 2),

            create_command("About", "Show a short introduction to the story.", "^(O HRE|ABOUT)$", 2),
            create_command("Version", "Game version number and author contact.", "^(VERZIA|VERSION)$", 2),

            create_command("Quit", "Quit the game.", "^(KONIEC|QUIT|EXIT)$", 2),
            create_command("Restart", "Restart the game.", "^(RESTART)$", 2),

            create_command("Save", "Save the state of the played game to path.", "^(ULOZ|SAVE)( (.*))?$", 4),
            create_command("Load", "Load a saved game from path.", "^(NAHRAJ|LOAD)( (.*))?$", 4),
        };

    // create container list out of commands
    struct container *first = create_container(NULL, COMMAND, commands[0]);
    for (size_t i = 1; i < ARRAY_SIZE(commands); i++)
    {
        create_container(first, COMMAND, commands[i]);
    }

    // create parser
    struct parser *new;
    MALLOC(1, new);

    ASSERT(first != NULL);

    new->commands = first;
    new->history = NULL;

    return new;
}

struct parser *destroy_parser(struct parser *parser)
{
    CHECK_NULL(parser);

    parser->history = destroy_containers(parser->history);
    parser->commands = destroy_containers(parser->commands);
    FREE(parser);

    return parser;
}

struct command *parse_input(struct parser *parser, char *input)
{
    CHECK_NULL(parser);
    CHECK_NULL(input);

    // remove whitespace
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
                // defensively free groups becuase of loading from history / command repetition
                FREE(cmd->groups[i]);

                if (groups[i].rm_so != -1 && groups[i].rm_eo != -1)
                {
                    cmd->groups[i] = strndup(input + groups[i].rm_so, (size_t)(groups[i].rm_eo - groups[i].rm_so));
                }
            }

            return cmd;
        }
        // handle error
        else if (rc != REG_NOMATCH)
        {
            char buffer[ERROR_BUFFER_SIZE];
            regerror(rc, &cmd->preg, buffer, sizeof(buffer));
            fprintf(stderr, "Error! Matching regular expression: '%s' failed.\n", buffer);
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