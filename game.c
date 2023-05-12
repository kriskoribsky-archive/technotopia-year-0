#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"

#include "world.h"

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

// game info
#define VERSION "1.0.0"
#define AUTHOR "Kristian Koribsky"
#define CONTACT "kristian.koribsky@student.tuke.sk | kriskoribsky.me"

// game settings
#define BACKPACK_CAPACITY 5

/**
 * Search for the item in the game's current room or backpack
 *
 * Finds and returns a pointer to the item in the game identified by it's name
 * Name is case insensitive. If such item doesn't exist in the game, NULL is
 * returned. The item itself remains in the game.
 * @param game game to search item in, first tries to search in room, then in backpack
 * @param name name of item to search for
 * @return Returns reference to the found item or NULL if item was not found.
 */
struct item *get_item_from_game(const struct game *game, char *name);

void play_game(struct game *game)
{
    // prepare
    char format[128], input[INPUT_BUFFER_SIZE];
    sprintf(format, "%%%ds", INPUT_BUFFER_SIZE - 1);

    struct room *previous_room = NULL;

    // game loop
    do
    {
        // render
        if (game->current_room != previous_room)
        {
            show_room(game->current_room);
        }

        // input
        printf("> ");
        if (scanf(format, input) != 1)
        {
            fprintf(stderr, "Error! Wrong command input.\n");
            exit(EXIT_FAILURE);
        }

        // update
        execute_command(game, parse_input(game->parser, input));

    } while (game->state == PLAYING);

    // TODO implement other game states
}

struct game *create_game()
{
    struct game *new;
    MALLOC(1, new);

    new->state = PLAYING;

    new->parser = create_parser();
    new->world = create_world();
    new->backpack = create_backpack(BACKPACK_CAPACITY);
    ASSERT(new->parser != NULL);
    ASSERT(new->world != NULL);
    ASSERT(new->backpack != NULL);

    new->current_room = new->world->room;

    return new;
}

struct game *destroy_game(struct game *game)
{
    CHECK_NULL(game);

    game->parser = destroy_parser(game->parser);
    game->world = destroy_world(game->world);
    game->backpack = destroy_backpack(game->backpack);
    game->current_room = NULL; // room is freed by destroying world
    FREE(game);

    return game;
}

void execute_command(struct game *game, struct command *command)
#define IS_COMMAND(target) (strcmp(command->name, (target)) == 0)
{
    ASSERT(game != NULL && command != NULL);
    if (game == NULL || command == NULL)
    {
        return;
    }

    if (IS_COMMAND("PRESKUMAJ"))
    {
        char *name;
        struct item *item;

        if ((name = command->groups[1]) == NULL)
        {
            printf("Neviem, co chces preskumat.\n");
        }
        else if ((item = get_item_from_game(game, name)) == NULL)
        {
            printf("Predmet ktory chces preskumat sa nenachadza v miestnosti ani v batohu.\n");
        }
        else
        {
            if (~(item->properties & EXAMINABLE))
            {
                printf("Predmet sa neda preskumat.\n");
            }
            else
            {
                printf("Predmet %s:\n", item->name);
                printf("\t%s\n", item->description);
            }
        }
    }
    else if (IS_COMMAND("VEZMI"))
    {
        char *name;
        struct item *item;

        if ((name = command->groups[1]) == NULL)
        {
            printf("Neviem, co chces vziat.\n");
        }
        else if ((item = get_item_from_room(game->current_room, name)) == NULL)
        {
            printf("Predmet ktory chces vziat sa nenachadza v miestnosti.\n");
        }
        else
        {
            if (add_item_to_backpack(game->backpack, item) == false)
            {
                printf("Batoh je plny alebo predmet sa neda presuvat.\n");
            }
            else
            {
                delete_item_from_room(game->current_room, item);
            }
        }
    }
    else if (IS_COMMAND("POLOZ"))
    {
        char *name;
        struct item *item;

        if ((name = command->groups[1]) == NULL)
        {
            printf("Neviem, co chces polozit.\n");
        }
        else if ((item = get_item_from_backpack(game->backpack, name)) == NULL)
        {
            printf("Predmet ktory chces polozit sa nenachadza v tvojom batohu.\n");
        }
        else
        {
            add_item_to_room(game->current_room, item);
            delete_item_from_backpack(game->backpack, item);
        }
    }
    // TODO implement pouzi
    else if (IS_COMMAND("POUZI"))
    {
    }

    else if (IS_COMMAND("ROZHLIADNI SA"))
    {
        show_room(game->current_room);
    }
    else if (IS_COMMAND("INVENTAR"))
    {
        printf("Predmety v batohu:\n");
        for (struct container *current = game->backpack->items; current != NULL; current = current->next)
        {
            printf("\t%s\n", current->item->name);
        }
    }

    else if (IS_COMMAND("SEVER"))
    {
        if (game->current_room->north == NULL)
        {
            printf("Smerom na sever nevedie ziadna cesta.\n");
        }
        else
        {
            game->current_room = game->current_room->north;
        }
    }
    else if (IS_COMMAND("JUH"))
    {
        if (game->current_room->south == NULL)
        {
            printf("Smerom na juh nevedie ziadna cesta.\n");
        }
        else
        {
            game->current_room = game->current_room->south;
        }
    }
    else if (IS_COMMAND("VYCHOD"))
    {
        if (game->current_room->east == NULL)
        {
            printf("Smerom na vychod nevedie ziadna cesta.\n");
        }
        else
        {
            game->current_room = game->current_room->east;
        }
    }
    else if (IS_COMMAND("ZAPAD"))
    {
        if (game->current_room->west == NULL)
        {
            printf("Smerom na zapad nevedie ziadna cesta.\n");
        }
        else
        {
            game->current_room = game->current_room->west;
        }
    }

    else if (IS_COMMAND("O HRE"))
    {
        printf("Kde bolo tam bolo, ...\n");
    }
    else if (IS_COMMAND("PRIKAZY"))
    {
        printf("Prikazy:\n");
        for (struct container *current = game->parser->commands; current != NULL; current = current->next)
        {
            printf("\t%s\t->\t%s\n", current->command->name, current->command->description);
        }
    }
    else if (IS_COMMAND("VERZIA"))
    {
        printf("Hotel Transylvania.\n");
        printf("Od strachu sa vam budu jezit chlpy na chrbte, muhahhaa!\n");
        printf("============================================================\n");
        printf("Verzia: %s\n", VERSION);
        printf("Autor: %s\n", AUTHOR);
        printf("Kontakt: %s\n", CONTACT);
    }

    else if (IS_COMMAND("KONIEC"))
    {
        game->state = GAMEOVER;
    }
    else if (IS_COMMAND("RESTART"))
    {
        game->state = RESTART;
    }

    // TODO lastly implement game saving
    else if (IS_COMMAND("ULOZ"))
    {
    }
    else if (IS_COMMAND("NAHRAJ"))
    {
    }
    else
    {
        fprintf(stderr, "Error! Command '%s' not implemented.\n", command->name);
        exit(EXIT_FAILURE);
    }
#undef IS_COMMAND
}

struct item *get_item_from_game(const struct game *game, char *name)
{
    ASSERT(game != NULL && name != NULL);
    CHECK_NULL(game);
    CHECK_NULL(name);

    struct item *item;
    return (item = get_item_from_room(game->current_room, name)) != NULL ? item : get_item_from_backpack(game->backpack, name);
}