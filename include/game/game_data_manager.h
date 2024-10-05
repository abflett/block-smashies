#ifndef GAME_DATA_MANAGER_H
#define GAME_DATA_MANAGER_H

#include "game_data.h"
#include "kvec.h"

typedef struct GameDataManager
{
    GameData game_data;
    kvec_t(GameDataSummary *) * summaries;
    int summaries_count;

    void (*add_player)(void);    // change player count and update active ships
    void (*remove_player)(void); // change player cound and update active ships
    void (*save_data)(void);
    void (*create_game_data)(void); // create a new default game data with starting attributes
    void (*load_game_data)(int id); // create a game data from an existing object from json

    void (*load_game_data_summaries)(void);

    void (*cleanup)(void);
} GameDataManager;

#endif