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

    // create list of available commands
    struct container *first = NULL;

    create_container(first, COMMAND, create_command("PRESKUMAJ", "Opis predmetu v batohu/miestnosti.", "PRESKUMAJ|SEARCH (.*)", 2));
    create_container(first, COMMAND, create_command("VEZMI", "Vložíť predmet z miestnosti do batohu.", "VEZMI|TAKE (.*)", 2));
    create_container(first, COMMAND, create_command("POLOZ", "Položíť predmet z batohu do miestnosti.", "POLOZ|PUT (.*)", 2));
    create_container(first, COMMAND, create_command("POUZI", "Použiť predmet z batohu alebo miestnosti.", "POUZI|USE (.*)", 2));

    create_container(first, COMMAND, create_command("ROZHLIADNI SA", "Informácie o miestnosti.", "ROZHLIADNI SA|LOOK", 1));
    create_container(first, COMMAND, create_command("INVENTAR", "Zobrazíť obsah batohu.", "INVENTAR|INVENTORY|I", 1));

    create_container(first, COMMAND, create_command("SEVER", "Ísť smerom na sever od aktuálnej pozície.", "SEVER|S", 1));
    create_container(first, COMMAND, create_command("JUH", "Ísť smerom na juh od aktuálnej pozície.", "JUH|J", 1));
    create_container(first, COMMAND, create_command("VYCHOD", "Ísť smerom na východ od aktuálnej pozície.", "VYCHOD|V", 1));
    create_container(first, COMMAND, create_command("ZAPAD", "Ísť smerom na západ od aktuálnej pozície.", "ZAPAD|Z", 1));

    create_container(first, COMMAND, create_command("O HRE", "Zobraziť krátky úvod do príbehu.", "O HRE|ABOUT", 1));
    create_container(first, COMMAND, create_command("PRIKAZY", "Zoznam všetkých príkazov hry.", "PRIKAZY|HELP|POMOC", 1));
    create_container(first, COMMAND, create_command("VERZIA", "Číslo verzie hry a kontakt na autora.", "VERZIA|VERSION", 1));

    create_container(first, COMMAND, create_command("KONIEC", "Ukončiť hru.", "KONIEC|QUIT|EXIT", 1));
    create_container(first, COMMAND, create_command("RESTART", "Spustíť hru od začiatku.", "RESTART", 1));

    create_container(first, COMMAND, create_command("ULOZ", "Uložíť stav rozohratej hry na disk.", "ULOZ|SAVE (.*)?", 2));
    create_container(first, COMMAND, create_command("NAHRAJ", "Nahrať uloženú hru z disku.", "NAHRAJ|LOAD (.*)?", 2));

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