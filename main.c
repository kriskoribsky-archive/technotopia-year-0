#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"

int main(void)
{
    struct game *game = create_game();

    play_game(game);

    destroy_game(game);

    return EXIT_SUCCESS;
}