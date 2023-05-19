#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "game.h"

int main(void)
{
    struct game *game = create_game();

    play_game(game);

    switch (game->state)
    {
    case SOLVED:
        printf("Contratulations, you won!\n");
        break;

    case GAMEOVER:
        break;

    case RESTART:
        destroy_game(game);
        return main();

    case PLAYING:
        fprintf(stderr, "Error! Game exited prematurely.\n");
        exit(EXIT_FAILURE);
    }

    destroy_game(game);
    return EXIT_SUCCESS;
}