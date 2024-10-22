#ifndef GAME_DATA_SUMMARY_H
#define GAME_DATA_SUMMARY_H

#include "game_data.h"
#include "kvec.h"
#include "uuid4.h"

typedef struct GameDataSummary
{
    char uuid[UUID4_LEN];
    char name[15];
    int operation_complete;
    int mission_complete;
    int high_score;
    int player_count;
    int currency_collected;
    int orb_retrievers;
    Difficulty difficulty;
} GameDataSummary;

typedef struct GameDataSummaries
{
    kvec_t(GameDataSummary) summaries;

    void (*delete)(struct GameDataSummaries *game_data_summaries, const char *uuid); // delete data from json data and update summaries
    void (*cleanup)(struct GameDataSummaries *game_data_summaries);
} GameDataSummaries;

GameDataSummaries *create_game_data_summaries(void);

#endif