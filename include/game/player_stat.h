#ifndef PLAYER_STAT_H
#define PLAYER_STAT_H

typedef struct PlayerStat
{
    int player_num;
    float energy;
    float force_cooldown;
    float heat;
    float boost_cooldown;
    float velocity;
} PlayerStat;

PlayerStat create_player_stat(const int player_num);

#endif