#define _POSIX_C_SOURCE 200809L
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
            create_command("PRESKUMAJ", "Opis predmetu v batohu/miestnosti.", "^(PRESKUMAJ|SEARCH)( .*)?$", 3),
            create_command("VEZMI", "Vložíť predmet z miestnosti do batohu.", "^(VEZMI|TAKE)( .*)?$", 3),
            create_command("POLOZ", "Položíť predmet z batohu do miestnosti.", "^(POLOZ|PUT)( .*)?$", 3),
            create_command("POUZI", "Použiť predmet z batohu alebo miestnosti.", "^(POUZI|USE)( .*)?$", 3),

            create_command("ROZHLIADNI SA", "Informácie o miestnosti.", "^(ROZHLIADNI SA|LOOK)$", 2),
            create_command("INVENTAR", "Zobrazíť obsah batohu.", "^(INVENTAR|INVENTORY|I)$", 2),
            create_command("PRIKAZY", "Zoznam všetkých príkazov hry.", "^(PRIKAZY|HELP|POMOC)$", 2),

            create_command("SEVER", "Ísť smerom na sever od aktuálnej pozície.", "^(SEVER|S)$", 2),
            create_command("JUH", "Ísť smerom na juh od aktuálnej pozície.", "^(JUH|J)$", 2),
            create_command("VYCHOD", "Ísť smerom na východ od aktuálnej pozície.", "^(VYCHOD|V)$", 2),
            create_command("ZAPAD", "Ísť smerom na západ od aktuálnej pozície.", "^(ZAPAD|Z)$", 2),

            create_command("O HRE", "Zobraziť krátky úvod do príbehu.", "^(O HRE|ABOUT)$", 2),
            create_command("VERZIA", "Číslo verzie hry a kontakt na autora.", "^(VERZIA|VERSION)$", 2),

            create_command("KONIEC", "Ukončiť hru.", "^(KONIEC|QUIT|EXIT)$", 2),
            create_command("RESTART", "Spustíť hru od začiatku.", "^(RESTART)$", 2),

            create_command("ULOZ", "Uložíť stav rozohratej hry na disk.", "^(ULOZ|SAVE)( .*)?$", 4),
            create_command("NAHRAJ", "Nahrať uloženú hru z disku.", "^(NAHRAJ|LOAD)( .*)?$", 3),
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
                if (groups[i].rm_so != -1 && groups[i].rm_eo != -1)
                {
                    char *dup = strndup(input + groups[i].rm_so, (size_t)(groups[i].rm_eo - groups[i].rm_so));
                    cmd->groups[i] = strdup(trim(dup));
                    FREE(dup);
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