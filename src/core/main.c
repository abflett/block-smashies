#include "game.h"

int main(void)
{
    const Game *game = create_game("Block Smashies");
    game->run();
    return 0;
}