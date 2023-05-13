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
#define NAME "TECHNOTOPIA: YEAR 0"
#define VERSION "1.0.0"
#define AUTHOR "Kristian Koribsky"
#define CONTACT "kristian.koribsky@student.tuke.sk | kriskoribsky.me"

// game settings
#define BACKPACK_CAPACITY 5

// commands
void command_inspect(struct game *game, struct command *command);
void command_take(struct game *game, struct command *command);
void command_put(struct game *game, struct command *command);
void command_use(struct game *game, struct command *command);

void command_look(struct game *game, struct command *command);
void command_inventory(struct game *game, struct command *command);

void command_north(struct game *game, struct command *command);
void command_south(struct game *game, struct command *command);
void command_east(struct game *game, struct command *command);
void command_west(struct game *game, struct command *command);

void command_about(struct game *game, struct command *command);
void command_commands(struct game *game, struct command *command);
void command_version(struct game *game, struct command *command);

void command_quit(struct game *game, struct command *command);
void command_restart(struct game *game, struct command *command);

void command_save(struct game *game, struct command *command);
void command_load(struct game *game, struct command *command);

void play_game(struct game *game)
{
    // prepare
    char format[128], input[INPUT_BUFFER_SIZE];
    sprintf(format, "%%%ds", INPUT_BUFFER_SIZE - 1);

    struct room *previous_room = NULL;

    // game loop
    while (game->state == PLAYING)
    {
        // render
        if (game->current_room != previous_room)
        {
            show_room(game->current_room);
            previous_room = game->current_room;
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
    }

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
#define REGISTER_COMMAND(cmd, callback)        \
    {                                          \
        if (strcmp(command->name, (cmd)) == 0) \
        {                                      \
            return callback(game, command);    \
        }                                      \
    }
{
    ASSERT(game != NULL && command != NULL);
    if (game == NULL || command == NULL)
    {
        return;
    }

    REGISTER_COMMAND("PRESKUMAJ", command_inspect);
    REGISTER_COMMAND("VEZMI", command_take);
    REGISTER_COMMAND("POLOZ", command_put);
    REGISTER_COMMAND("POUZI", command_use);

    REGISTER_COMMAND("ROZHLIADNI SA", command_look);
    REGISTER_COMMAND("INVENTAR", command_inventory);

    REGISTER_COMMAND("SEVER", command_north);
    REGISTER_COMMAND("JUH", command_south);
    REGISTER_COMMAND("VYCHOD", command_east);
    REGISTER_COMMAND("ZAPAD", command_west);

    REGISTER_COMMAND("O HRE", command_about);
    REGISTER_COMMAND("PRIKAZY", command_commands);
    REGISTER_COMMAND("VERZIA", command_version);

    REGISTER_COMMAND("KONIEC", command_quit);
    REGISTER_COMMAND("RESTART", command_restart);

    REGISTER_COMMAND("ULOZ", command_save);
    REGISTER_COMMAND("NAHRAJ", command_load);

    // command created, but not implemented (programming exception)
    fprintf(stderr, "Error! Command '%s' not implemented.\n", command->name);
    exit(EXIT_FAILURE);

#undef REGISTER_COMMAND
}

void command_inspect(struct game *game, struct command *command)
{
    char *name;
    struct item *item;

    if ((name = command->groups[1]) == NULL)
    {
        printf("Neviem, co chces preskumat.\n");
        return;
    }
    if ((item = get_item_from_room(game->current_room, name)) == NULL)
    {
        if ((item = get_item_from_backpack(game->backpack, name)) == NULL)
        {
            printf("Predmet ktory chces preskumat sa nenachadza v miestnosti ani v batohu.\n");
            return;
        }
    }
    if (~(item->properties & EXAMINABLE))
    {
        printf("Predmet sa neda preskumat.\n");
        return;
    }

    printf("Predmet %s:\n", item->name);
    printf("\t%s\n", item->description);
}

void command_take(struct game *game, struct command *command)
{
    char *name;
    struct item *item;

    if ((name = command->groups[1]) == NULL)
    {
        printf("Neviem, co chces vziat.\n");
        return;
    }
    if ((item = get_item_from_room(game->current_room, name)) == NULL)
    {
        printf("Predmet ktory chces vziat sa nenachadza v miestnosti.\n");
        return;
    }
    if (add_item_to_backpack(game->backpack, item) == false)
    {
        printf("Batoh je plny alebo predmet sa neda presuvat.\n");
        return;
    }

    delete_item_from_room(game->current_room, item);
}

void command_put(struct game *game, struct command *command)
{
    char *name;
    struct item *item;

    if ((name = command->groups[1]) == NULL)
    {
        printf("Neviem, co chces polozit.\n");
        return;
    }
    if ((item = get_item_from_backpack(game->backpack, name)) == NULL)
    {
        printf("Predmet ktory chces polozit sa nenachadza v tvojom batohu.\n");
        return;
    }

    add_item_to_room(game->current_room, item);
    delete_item_from_backpack(game->backpack, item);
}

void command_use(struct game *game, struct command *command)
{
}

void command_look(struct game *game, struct command *command)
{
    // show items
    if (game->current_room->items)
    {
        printf("Predmety v tvojej blizkosti:\n");
        for (struct container *current = game->current_room->items; current != NULL; current = current->next)
        {
            printf("\t%s\n", current->item->name);
        }
    }

    // exits
    printf("Možné východy z miestnosti:\n");
    if (game->current_room->north)
    {
        printf("\tSever -> %s\n", game->current_room->north->name);
    }
    if (game->current_room->south)
    {
        printf("\tJuh -> %s\n", game->current_room->south->name);
    }
    if (game->current_room->east)
    {
        printf("\tVýchod -> %s\n", game->current_room->east->name);
    }
    if (game->current_room->west)
    {
        printf("\tZápad -> %s\n", game->current_room->west->name);
    }
}

void command_inventory(struct game *game, struct command *command)
{
    printf("Predmety v batohu:\n");
    for (struct container *current = game->backpack->items; current != NULL; current = current->next)
    {
        printf("\t%s\n", current->item->name);
    }
}

void command_north(struct game *game, struct command *command)
{
    if (game->current_room->north == NULL)
    {
        printf("Smerom na sever nevedie ziadna cesta.\n");
        return;
    }

    game->current_room = game->current_room->north;
}

void command_south(struct game *game, struct command *command)
{
    if (game->current_room->south == NULL)
    {
        printf("Smerom na juh nevedie ziadna cesta.\n");
        return;
    }

    game->current_room = game->current_room->south;
}
void command_east(struct game *game, struct command *command)
{
    if (game->current_room->east == NULL)
    {
        printf("Smerom na vychod nevedie ziadna cesta.\n");
        return;
    }

    game->current_room = game->current_room->east;
}

void command_west(struct game *game, struct command *command)
{
    if (game->current_room->west == NULL)
    {
        printf("Smerom na zapad nevedie ziadna cesta.\n");
        return;
    }

    game->current_room = game->current_room->west;
}

void command_about(struct game *game, struct command *command)
{
    printf("Kde bolo tam bolo, ...\n");
}

void command_commands(struct game *game, struct command *command)
{
    printf("Prikazy:\n");
    for (struct container *current = game->parser->commands; current != NULL; current = current->next)
    {
        printf("\t%s\t->\t%s\n", current->command->name, current->command->description);
    }
}

void command_version(struct game *game, struct command *command)
{
    printf("%s\n", NAME);
    printf("\n");
    printf("============================================================\n");
    printf("Version: %s\n", VERSION);
    printf("Author: %s\n", AUTHOR);
    printf("Contact: %s\n", CONTACT);
}

void command_quit(struct game *game, struct command *command)
{
    game->state = GAMEOVER;
}

void command_restart(struct game *game, struct command *command)
{
    game->state = RESTART;
}

void command_save(struct game *game, struct command *command)
{
}
void command_load(struct game *game, struct command *command)
{
}