#include "player_stat.h"

PlayerStat create_player_stat(int player_num)
{
    PlayerStat player;
    player.player_num = player_num;
    player.energy = 1.0f;
    player.force_cooldown = 1.0f;
    player.heat = 0.0f;
    player.boost_cooldown = 1.0f;

    return player;
}