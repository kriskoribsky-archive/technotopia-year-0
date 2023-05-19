#include "unity/src/unity.h"

#include "game.h"

void setUp(void);
void tearDown(void);

void test_execute_command_command_south_changes_current_room(void);
void test_execute_command_command_west_changes_current_room(void);

void test_execute_command_command_restart_changes_game_state_to_restart(void);
void test_execute_command_command_exit_changes_game_state_to_gameover(void);

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_execute_command_command_south_changes_current_room);
    RUN_TEST(test_execute_command_command_west_changes_current_room);

    RUN_TEST(test_execute_command_command_restart_changes_game_state_to_restart);
    RUN_TEST(test_execute_command_command_exit_changes_game_state_to_gameover);

    return UNITY_END();
}

void setUp(void)
{
}

void tearDown(void)
{
}

void test_execute_command_command_south_changes_current_room(void)
{
    struct game *game = create_game();
    struct room *north = create_room("Bakery", "The delightful smell of meat pies fills the air, making you hungry. The baker flashes a grin, as he slides a box marked \" Not Human Organs \" under a table with his foot.");
    struct room *south = create_room("Observation Deck", "You can see the entire town from the top of the Wizard Tower. Everybody looks like ants, especially the people transformed into ants by the wizards of the tower!");

    set_exits_from_room(north, NULL, south, NULL, NULL);
    set_exits_from_room(south, north, NULL, NULL, NULL);

    game->current_room = north;
    struct command *command = parse_input(game->parser, "JUH");
    execute_command(game, command);

    TEST_ASSERT_EQUAL_PTR(south, game->current_room);
    destroy_game(game);
}

void test_execute_command_command_west_changes_current_room(void)
{
    struct game *game = create_game();
    struct room *east = create_room("North Y Street", "The northern end of Y Street has really gone down hill. Pot holes are everywhere, as are stray cats, rats, and wombats.");
    struct room *west = create_room("Thief Guild", "The Thief Guild is a dark den of unprincipled types. You clutch your purse (though several other people here would like to clutch your purse as well).");

    set_exits_from_room(east, NULL, NULL, NULL, west);
    set_exits_from_room(west, NULL, NULL, east, NULL);

    game->current_room = east;
    struct command *command = parse_input(game->parser, "ZAPAD");
    execute_command(game, command);

    TEST_ASSERT_EQUAL_PTR(west, game->current_room);
    destroy_game(game);
}

void test_execute_command_command_restart_changes_game_state_to_restart(void)
{
    struct game *game = create_game();
    struct command *command = parse_input(game->parser, "RESTART");
    execute_command(game, command);

    TEST_ASSERT_EQUAL(RESTART, game->state);
    destroy_game(game);
}

void test_execute_command_command_exit_changes_game_state_to_gameover(void)
{
    struct game *game = create_game();
    struct command *command = parse_input(game->parser, "KONIEC");
    execute_command(game, command);

    TEST_ASSERT_EQUAL(GAMEOVER, game->state);
    destroy_game(game);
}