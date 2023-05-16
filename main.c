#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "game.h"

int main(void)
{
    // struct game *game = create_game();

    // play_game(game);

    // switch (game->state)
    // {
    // case SOLVED:
    //     printf("Contratulations, you won!\n");
    //     break;

    // case GAMEOVER:
    //     break;

    // case RESTART:
    //     destroy_game(game);
    //     return main();

    // case PLAYING:
    //     fprintf(stderr, "Error! Game exited prematurely.\n");
    //     exit(EXIT_FAILURE);
    // }

    // destroy_game(game);
    // return EXIT_SUCCESS;

    char *name = "PRESKUMAJ";
    char *description = "Preskuma zvoleny predmet.";
    struct command *command = create_command(name, description, NULL, 0);

    printf("comparison result: %d", strcasecmp(name, command->name) && strcasecmp(description, command->description));

}