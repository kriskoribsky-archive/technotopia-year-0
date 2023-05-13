#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // sleep

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

#define CHECK_NULL_VOID(ptr) \
    {                        \
        if ((ptr) == NULL)   \
        {                    \
            return;          \
        }                    \
    }

#define CLEAR_INPUT_BUFFER()                        \
    {                                               \
        int c;                                      \
        while ((c = getchar()) != '\n' && c != EOF) \
            ;                                       \
    }

// game info
#define NAME "TECHNOTOPIA: YEAR 0"
#define DESC "Change the world, one line of code at a time."
#define VERSION "1.0.0"
#define AUTHOR "Kristian Koribsky (2023)"
#define CONTACT "kristian.koribsky@student.tuke.sk | kriskoribsky.me"

// game settings
#define BACKPACK_CAPACITY 5
#define DEFAULT_SAVE "save.txt"

// commands
void command_inspect(struct game *game, struct command *command);
void command_take(struct game *game, struct command *command);
void command_put(struct game *game, struct command *command);
void command_use(struct game *game, struct command *command);

void command_look(struct game *game);
void command_inventory(struct game *game);
void command_commands(struct game *game);

void command_north(struct game *game);
void command_south(struct game *game);
void command_east(struct game *game);
void command_west(struct game *game);

void command_about(void);
void command_version(void);

void command_quit(struct game *game);
void command_restart(struct game *game);

void command_save(struct game *game, struct command *command);
void command_load(struct game *game, struct command *command);

void play_game(struct game *game)
{
    // print introductory info
    command_version();

    // initialize buffer
    char input[INPUT_BUFFER_SIZE];
    struct room *previous = NULL;

    // game loop
    while (game->state == PLAYING)
    {
        // render
        if (game->current_room != previous)
        {
            show_room((previous) = game->current_room);
        }

        // input
        printf("\n> ");
        if (fgets(input, INPUT_BUFFER_SIZE, stdin) == NULL)
        {
            fprintf(stderr, "\nEncountered EOF, gracefully exiting the game.\n");
            return;
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
            callback;                          \
            return;                            \
        }                                      \
    }
{
    ASSERT(game != NULL);
    CHECK_NULL_VOID(game);
    if (command == NULL)
    {
        printf("You do not have knowledge to do that.\n");
        return;
    }

    REGISTER_COMMAND("PRESKUMAJ", command_inspect(game, command));
    REGISTER_COMMAND("VEZMI", command_take(game, command));
    REGISTER_COMMAND("POLOZ", command_put(game, command));
    REGISTER_COMMAND("POUZI", command_use(game, command));

    REGISTER_COMMAND("ROZHLIADNI SA", command_look(game));
    REGISTER_COMMAND("INVENTAR", command_inventory(game));
    REGISTER_COMMAND("PRIKAZY", command_commands(game));

    REGISTER_COMMAND("SEVER", command_north(game));
    REGISTER_COMMAND("JUH", command_south(game));
    REGISTER_COMMAND("VYCHOD", command_east(game));
    REGISTER_COMMAND("ZAPAD", command_west(game));

    REGISTER_COMMAND("O HRE", command_about());
    REGISTER_COMMAND("VERZIA", command_version());

    REGISTER_COMMAND("KONIEC", command_quit(game));
    REGISTER_COMMAND("RESTART", command_restart(game));

    REGISTER_COMMAND("ULOZ", command_save(game, command));
    REGISTER_COMMAND("NAHRAJ", command_load(game, command));

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
    CHECK_NULL_VOID(game);
    CHECK_NULL_VOID(command);

    add_item_to_room(game->current_room, get_item_from_backpack(game->backpack, command->name));
}

void command_look(struct game *game)
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

void command_inventory(struct game *game)
{
    printf("Predmety v batohu:\n");
    for (struct container *current = game->backpack->items; current != NULL; current = current->next)
    {
        printf("\t%s\n", current->item->name);
    }
}

void command_commands(struct game *game)
{
    printf("Prikazy:\n");
    for (struct container *current = game->parser->commands; current != NULL; current = current->next)
    {
        printf("\t%s\t->\t%s\n", current->command->name, current->command->description);
    }
}

void command_north(struct game *game)
{
    if (game->current_room->north == NULL)
    {
        printf("Smerom na sever nevedie ziadna cesta.\n");
        return;
    }

    game->current_room = game->current_room->north;
}

void command_south(struct game *game)
{
    if (game->current_room->south == NULL)
    {
        printf("Smerom na juh nevedie ziadna cesta.\n");
        return;
    }

    game->current_room = game->current_room->south;
}
void command_east(struct game *game)
{
    if (game->current_room->east == NULL)
    {
        printf("Smerom na vychod nevedie ziadna cesta.\n");
        return;
    }

    game->current_room = game->current_room->east;
}

void command_west(struct game *game)
{
    if (game->current_room->west == NULL)
    {
        printf("Smerom na zapad nevedie ziadna cesta.\n");
        return;
    }

    game->current_room = game->current_room->west;
}

void command_about(void)
{
    printf("Kde bolo tam bolo, ...\n");
}

void command_version(void)
{
    printf("\n");
    printf("%s\n", NAME);
    printf("%s\n", DESC);
    printf("-----------------------------------------------\n");
    printf("Version %s\n", VERSION);
    printf("%s\n", AUTHOR);
    printf("%s\n", CONTACT);
    printf("\n");
    printf("\n");
}

void command_quit(struct game *game)
{
    game->state = GAMEOVER;
}

void command_restart(struct game *game)
{
    game->state = RESTART;
}

void command_save(struct game *game, struct command *command)
{
    FILE *fp;
    if ((fp = fopen(command->groups[2] != NULL ? command->groups[2] : DEFAULT_SAVE, "w")) == NULL)
    {
        fprintf(stderr, "Error! Cannot open file.\n");
        return;
    }

    // dump all commands from history text container
    for (struct container *current = game->parser->history; current != NULL; current = current->next)
    {
        fprintf(fp, "%s\n", current->text);
    }

    printf("Syncing data...\n");
    sleep(1);
    printf("Simulation waypoint set. You are now a part of history.\n");

    fclose(fp);
}
void command_load(struct game *game, struct command *command)
{
    FILE *fp;
    if ((fp = fopen(command->groups[2] != NULL ? command->groups[2] : DEFAULT_SAVE, "r")) == NULL)
    {
        printf("System error: Simulation data not found. Re-sync required.\n");
        return;
    }

    // clear current history
    game->parser->history = destroy_containers(game->parser->history);

    // execute loaded commands

    printf("Connecting to server...\n");
    sleep(1);
    printf("Simulation waypoint found. Your consciousness is now entwined.\n");

    fclose(fp);
}