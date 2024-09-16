#include "game.h"

int main(void)
{
    Game *game = create_game("Block Smashies", "settings.json", "assets/config/resources.json");
    game->run();
    return 0;
}