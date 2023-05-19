#define _POSIX_C_SOURCE 200809L // strdup
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

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

// game settings
#define BACKPACK_CAPACITY 5
#define DEFAULT_SAVE "save.txt"

// commands
void command_inspect(struct game *game, struct command *command);
void command_take(struct game *game, struct command *command);
void command_put(struct game *game, struct command *command);
void command_use(struct game *game, struct command *command);

void command_explore(struct game *game);
void command_inventory(struct game *game);
void command_commands(struct game *game);

void command_north(struct game *game, struct command *command);
void command_south(struct game *game, struct command *command);
void command_east(struct game *game, struct command *command);
void command_west(struct game *game, struct command *command);

void command_about(void);
void command_version(void);

void command_quit(struct game *game);
void command_restart(struct game *game);

void command_save(struct game *game, struct command *command);
void command_load(struct game *game, struct command *command);

// helpers
void save_to_history(struct game *game, struct command *command);

#define COMMAND_ENTIRE 0 // groups index of whole command
#define COMMAND_PARAM 3  // group index of param part
#define SAVE_COMMAND (save_to_history(game, command))
#define REGISTER_COMMAND(cmd, callback)            \
    {                                              \
        if (strcasecmp(command->name, (cmd)) == 0) \
        {                                          \
            callback;                              \
            return;                                \
        }                                          \
    }

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
            fprintf(stderr, "Encountered EOF, gracefully exiting the game.\n");
            return;
        }

        // update
        execute_command(game, parse_input(game->parser, input));
    }
}

struct game *create_game()
{
    struct game *new;
    MALLOC(1, new);

    new->state = PLAYING;
    new->parser = create_parser();
    new->world = create_world();
    new->backpack = create_backpack(BACKPACK_CAPACITY);

    assert(new->parser != NULL);
    assert(new->world != NULL);
    assert(new->backpack != NULL);

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
    CHECK_NULL_VOID(game);
    CHECK_NULL_VOID(game->parser);
    CHECK_NULL_VOID(game->world);
    CHECK_NULL_VOID(game->current_room);
    CHECK_NULL_VOID(game->backpack);
    if (command == NULL)
    {
        printf("You do not have knowledge to do that.\n");
        return;
    }

    // arena commands
    REGISTER_COMMAND("PRESKUMAJ", command_inspect(game, command));
    REGISTER_COMMAND("VEZMI", command_take(game, command));
    REGISTER_COMMAND("POLOZ", command_put(game, command));
    REGISTER_COMMAND("POUZI", command_use(game, command));

    REGISTER_COMMAND("ROZHLIADNI SA", command_explore(game));
    REGISTER_COMMAND("INVENTAR", command_inventory(game));
    REGISTER_COMMAND("PRIKAZY", command_commands(game));

    REGISTER_COMMAND("SEVER", command_north(game, command));
    REGISTER_COMMAND("JUH", command_south(game, command));
    REGISTER_COMMAND("VYCHOD", command_east(game, command));
    REGISTER_COMMAND("ZAPAD", command_west(game, command));

    REGISTER_COMMAND("O HRE", command_about());
    REGISTER_COMMAND("VERZIA", command_version());

    REGISTER_COMMAND("KONIEC", command_quit(game));
    REGISTER_COMMAND("RESTART", command_restart(game));

    REGISTER_COMMAND("ULOZ", command_save(game, command));
    REGISTER_COMMAND("NAHRAJ", command_load(game, command));

    // command created, but not implemented (programming exception)
    fprintf(stderr, "Error! Command '%s' not implemented.\n", command->name);
    exit(EXIT_FAILURE);
}

// commands
void command_inspect(struct game *game, struct command *command)
{
    char *name;
    struct item *item;

    if ((name = command->groups[COMMAND_PARAM]) == NULL)
    {
        printf("Please specify item you want to examine.\n");
        return;
    }
    if ((item = get_item_from_room(game->current_room, name)) == NULL)
    {
        if ((item = get_item_from_backpack(game->backpack, name)) == NULL)
        {
            printf("The item you want to examine is not in the room or in the backpack.\n");
            return;
        }
    }
    if ((item->properties & EXAMINABLE) == 0)
    {
        printf("Object is not examinable.\n");
        return;
    }

    printf("%s\n", item->description);
}

void command_take(struct game *game, struct command *command)
{
    char *name;
    struct item *item;

    if ((name = command->groups[COMMAND_PARAM]) == NULL)
    {
        printf("Please specify item you want to take.\n");
        return;
    }
    if ((item = get_item_from_room(game->current_room, name)) == NULL)
    {
        printf("The item you want to take is not in the room.\n");
        return;
    }
    if (add_item_to_backpack(game->backpack, item) == false)
    {
        printf("The backpack is full or the object is not movable.\n");
        return;
    }

    delete_item_from_room(game->current_room, item);
    SAVE_COMMAND;
}

void command_put(struct game *game, struct command *command)
{
    char *name;
    struct item *item;

    if ((name = command->groups[COMMAND_PARAM]) == NULL)
    {
        printf("Please specify item you want to put down.\n");
        return;
    }
    if ((item = get_item_from_backpack(game->backpack, name)) == NULL)
    {
        printf("The item you want to put down is not in your backpack.\n");
        return;
    }

    add_item_to_room(game->current_room, item);
    delete_item_from_backpack(game->backpack, item);
    SAVE_COMMAND;
}

void command_use(struct game *game, struct command *command)
{
    char *name;
    struct item *item;

    if ((name = command->groups[COMMAND_PARAM]) == NULL)
    {
        printf("Please specify item you want to use.\n");
        return;
    }
    if ((item = get_item_from_room(game->current_room, name)) == NULL)
    {
        if ((item = get_item_from_backpack(game->backpack, name)) == NULL)
        {
            printf("The item you want to use is not in the room or in the backpack.\n");
            return;
        }
    }
    if ((item->properties & USABLE) == 0)
    {
        printf("This item cannot be used.\n");
        return;
    }

    get_room(game->world, "room 1");
    printf("Used\n");

    SAVE_COMMAND;
}

void command_explore(struct game *game)
{
    // exits
    printf("Exits:\n");
    if (game->current_room->north)
    {
        printf("   North -> %s\n", game->current_room->north->name);
    }
    if (game->current_room->south)
    {
        printf("   South -> %s\n", game->current_room->south->name);
    }
    if (game->current_room->east)
    {
        printf("   East  -> %s\n", game->current_room->east->name);
    }
    if (game->current_room->west)
    {
        printf("   West  -> %s\n", game->current_room->west->name);
    }

    // show items
    if (game->current_room->items)
    {
        printf("Items:\n");
        for (struct container *current = game->current_room->items; current != NULL; current = current->next)
        {
            printf("   %s\n", current->item->name);
        }
    }
}

void command_inventory(struct game *game)
{
    if (game->backpack->items == NULL)
    {
        printf("Your backpack is empty.");
        return;
    }

    for (struct container *current = game->backpack->items; current != NULL; current = current->next)
    {
        printf("%s\n", current->item->name);
    }
}

void command_commands(struct game *game)
{
    for (struct container *current = game->parser->commands; current != NULL; current = current->next)
    {
        printf("%-20s%s\n", current->command->name, current->command->description);
    }
}

void command_north(struct game *game, struct command *command)
{
    if (game->current_room->north == NULL)
    {
        printf("There is no exit there.\n");
        return;
    }

    game->current_room = game->current_room->north;
    SAVE_COMMAND;

    if (strcmp(game->current_room->name, "temp n") == 0)
    {
        game->state = SOLVED;
    }
}

void command_south(struct game *game, struct command *command)
{
    if (game->current_room->south == NULL)
    {
        printf("There is no exit there.\n");
        return;
    }

    game->current_room = game->current_room->south;
    SAVE_COMMAND;
}
void command_east(struct game *game, struct command *command)
{
    if (game->current_room->east == NULL)
    {
        printf("There is no exit there.\n");
        return;
    }

    game->current_room = game->current_room->east;
    SAVE_COMMAND;
}

void command_west(struct game *game, struct command *command)
{
    if (game->current_room->west == NULL)
    {
        printf("There is no exit there.\n");
        return;
    }

    game->current_room = game->current_room->west;
    SAVE_COMMAND;
}

void command_about(void)
{
    printf("It is the year 2099, humanity has made more progress in the last 50 years than in the previous millennium.\n"
           "This was made possible after an AI revolution in 2023, which led to an exponential progression in technology,\n"
           "defying all laws such as Moore's Law, Nielsen's Law, and Bell's Law.\n\n"

           "Despite being 96 years old, your entire body has been augmented with bionic limbs and top-of-the-line health\n"
           "technology. Just last week, you had your stem cell appointment and GATA6 gene mod.\n"
           "World's life expectancy has more than doubled since the 20s.\n\n"

           "However, in terms of politics, the world has not changed much since 2023. Standard political structures\n"
           "and systems of control still reign supreme, and the gap between the haves and the have-nots has only grown wider.\n\n"

           "As you reminisce while tinkering with your now-antiquated Arduino UNO in your apartment, your brain's Neuralink\n"
           "receives a new message for you from an unknown sender.\n");
}

void command_version(void)
{
    printf("\n");
    printf("%s\n", "TECHNOTOPIA: YEAR 0");
    printf("%s\n", "Change the world, one line of code at a time.");
    printf("-----------------------------------------------\n");
    printf("Version %s\n", "1.0.0");
    printf("%s\n", "Kristian Koribsky (2023)");
    printf("%s\n", "kristian.koribsky@student.tuke.sk | kriskoribsky.me");
    printf("\n");
    printf("\n");
}

void command_quit(struct game *game)
{
    printf("Shutting down consciousness...\n");
    game->state = GAMEOVER;
}

void command_restart(struct game *game)
{
    printf("Rewinding time...\n");
    game->state = RESTART;
}

void command_save(struct game *game, struct command *command)
{
    FILE *fp;
    if ((fp = fopen(command->groups[COMMAND_PARAM] != NULL ? command->groups[COMMAND_PARAM] : DEFAULT_SAVE, "w")) == NULL)
    {
        fprintf(stderr, "Error! Cannot open file.\n");
        return;
    }

    // dump all commands from history text container
    for (struct container *current = game->parser->history; current != NULL; current = current->next)
    {
        fprintf(fp, "%s\n", current->text);
    }
    fclose(fp);

    printf("Syncing data...\n");
    printf("Simulation waypoint set. You are now a part of history.\n");
}
void command_load(struct game *game, struct command *command)
{
    FILE *fp;
    if ((fp = fopen(command->groups[COMMAND_PARAM] != NULL ? command->groups[COMMAND_PARAM] : DEFAULT_SAVE, "r")) == NULL)
    {
        printf("System error: Simulation data not found. Re-sync required.\n");
        return;
    }

    // reload game
    game->parser = destroy_parser(game->parser);
    game->world = destroy_world(game->world);
    game->backpack = destroy_backpack(game->backpack);

    game->parser = create_parser();
    game->world = create_world();
    game->backpack = create_backpack(BACKPACK_CAPACITY);
    assert(game->parser != NULL);
    assert(game->world != NULL);
    assert(game->backpack != NULL);

    game->current_room = game->world->room;

    // execute loaded commands
    char input[INPUT_BUFFER_SIZE];
    while (fgets(input, INPUT_BUFFER_SIZE, fp) != NULL)
    {
        execute_command(game, parse_input(game->parser, input));
    }
    fclose(fp);

    printf("Connecting to server...\n");
    printf("Simulation waypoint found. Your consciousness is now entwined.\n");
}

// helpers
void save_to_history(struct game *game, struct command *command)
{
    ASSERT(game != NULL && command != NULL && command->groups[COMMAND_ENTIRE] != NULL);
    CHECK_NULL_VOID(game);
    CHECK_NULL_VOID(command);

    struct container *c = create_container(game->parser->history, TEXT, strdup(command->groups[COMMAND_ENTIRE]));
    game->parser->history = game->parser->history != NULL ? game->parser->history : c;
}
