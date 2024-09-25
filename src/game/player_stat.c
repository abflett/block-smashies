#include "player_stat.h"

PlayerStat create_player_stat(int player_num)
{
    PlayerStat player;
    player.player_num = player_num;
    player.energy = 0.8f;
    player.force_cooldown = 0.5f;
    player.heat = 0.3f;
    player.boost_cooldown = 0.8f;
    player.velocity = 0.4f;

    return player;
}