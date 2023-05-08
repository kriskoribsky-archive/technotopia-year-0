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

#define BACKPACK_CAPACITY 5

void play_game(struct game *game)
{
    // prepare
    char format[128], input[INPUT_BUFFER_SIZE];
    sprintf(format, "> %%%ds", INPUT_BUFFER_SIZE - 1);

    struct room *previous_room = NULL;

    // loop
    do
    {
        // render
        if (game->current_room != previous_room)
        {
            show_room(game->current_room);
        }

        // input
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
{
    ASSERT(game != NULL);
    if (command == NULL)
    {
        return;
    }

    // TODO come up with some better sulution for this
    if (strcmp(command->name, "KONIEC") == 0)
    {
        game->state = GAMEOVER;
    }
    else if (strcmp(command->name, "SEVER") == 0)
    {
        game->current_room = game->current_room->north == NULL ? game->current_room : game->current_room->north;
    }
    else if (strcmp(command->name, "JUH") == 0)
    {
        game->current_room = game->current_room->south == NULL ? game->current_room : game->current_room->south;
    }
    else if (strcmp(command->name, "VYCHOD") == 0)
    {
        game->current_room = game->current_room->east == NULL ? game->current_room : game->current_room->east;
    }
    else if (strcmp(command->name, "ZAPAD") == 0)
    {
        game->current_room = game->current_room->west == NULL ? game->current_room : game->current_room->west;
    }
    else if (strcmp(command->name, "ROZHLIADNI SA") == 0)
    {
        show_room(game->current_room);
    }
    else if (strcmp(command->name, "PRIKAZY") == 0)
    {
    }
    else if (strcmp(command->name, "VERZIA") == 0)
    {
    }
    else if (strcmp(command->name, "RESTART") == 0)
    {
    }
    else if (strcmp(command->name, "O HRE") == 0)
    {
    }
    else if (strcmp(command->name, "VEZMI") == 0)
    {
    }
    else if (strcmp(command->name, "POLOZ") == 0)
    {
    }
    else if (strcmp(command->name, "INVENTAR") == 0)
    {
    }
    else if (strcmp(command->name, "POUZI") == 0)
    {
    }
    else if (strcmp(command->name, "PRESKUMAJ") == 0)
    {
    }
    else if (strcmp(command->name, "ULOZ") == 0)
    {
    }
    else if (strcmp(command->name, "NAHRAJ") == 0)
    {
    }
    else
    {
        fprintf(stderr, "Error! Command '%s' not implemented.\n", command->name);
        exit(EXIT_FAILURE);
    }
}