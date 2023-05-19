#define _POSIX_C_SOURCE 200809L // strdup
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "helpers.h"
#include "parser.h"

#define REG_MATCH 0
#define ERROR_BUFFER_SIZE 256

struct parser *create_parser()
{
    struct command *commands[] = {

        create_command("PRESKUMAJ", "Opis predmetu v batohu/miestnosti.", "^(PRESKUMAJ)( (.*))?$", 4),
        create_command("VEZMI", "Vložíť predmet z miestnosti do batohu.", "^(VEZMI)( (.*))?$", 4),
        create_command("POLOZ", "Položíť predmet z batohu do miestnosti.", "^(POLOZ)( (.*))?$", 4),
        create_command("POUZI", "Použiť predmet z batohu alebo miestnosti.", "^(POUZI)( (.*))?$", 4),

        create_command("ROZHLIADNI SA", "Informácie o miestnosti.", "^(ROZHLIADNI SA)$", 2),
        create_command("INVENTAR", "Zobrazíť obsah batohu.", "^(INVENTAR|INVENTORY|I)$", 2),
        create_command("PRIKAZY", "Zoznam všetkých príkazov hry.", "^(PRIKAZY|HELP|POMOC)$", 2),

        create_command("SEVER", "Ísť smerom na sever od aktuálnej pozície.", "^(SEVER|S)$", 2),
        create_command("JUH", "Ísť smerom na juh od aktuálnej pozície.", "^(JUH|J)$", 2),
        create_command("VYCHOD", "Ísť smerom na východ od aktuálnej pozície.", "^(VYCHOD|V)$", 2),
        create_command("ZAPAD", "Ísť smerom na západ od aktuálnej pozície.", "^(ZAPAD|Z)$", 2),

        create_command("O HRE", "Zobraziť krátky úvod do príbehu.", "^(O HRE|ABOUT)$", 2),
        create_command("VERZIA", "Číslo verzie hry a kontakt na autora.", "^(VERZIA)$", 2),

        create_command("KONIEC", "Ukončiť hru.", "^(KONIEC|QUIT|EXIT)$", 2),
        create_command("RESTART", "Spustíť hru od začiatku.", "^(RESTART)$", 2),

        create_command("NAHRAJ", "Nahrať uloženú hru z disku.", "^(NAHRAJ|LOAD)( (.*))?$", 4),
        create_command("ULOZ", "Uložíť stav rozohratej hry na disk.", "^(ULOZ|SAVE)( (.*))?$", 4),

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

    new->commands = first;
    new->history = NULL;

    assert(new->commands != NULL);
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
    CHECK_EMPTY(input);

    // parse input
    size_t len = strlen(input) + 1;
    char parsed_input[len];
    trim(parsed_input, len, input);

    CHECK_NULL(parsed_input);
    CHECK_EMPTY(parsed_input);

    // list trough available commands
    for (struct container *current = parser->commands; current != NULL; current = current->next)
    {
        struct command *cmd = current->command;

        int rc;
        regmatch_t groups[cmd->nmatch];
        if ((rc = regexec(&cmd->preg, parsed_input, cmd->nmatch, groups, 0)) == REG_MATCH)
        {
            // save matched groups
            for (size_t i = 0; i < cmd->nmatch; i++)
            {
                // free previously parsed groups
                FREE(cmd->groups[i]);

                if (groups[i].rm_so != -1 && groups[i].rm_eo != -1)
                {

                    cmd->groups[i] = strndup(parsed_input + groups[i].rm_so, (size_t)(groups[i].rm_eo - groups[i].rm_so));
                    assert(cmd->groups[i] != NULL);
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