#include "game.h"

int main(void)
{
    Game *game = create_game("Block Smashies");
    game->run();
    return 0;
}