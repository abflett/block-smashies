#include "game_data_summary.h"

static void delete_game_data_summary(struct GameDataSummaries *game_data_summaries, const char *uuid)
{
    // delete json data based on uuid
}

static void cleanup_game_data_summary(struct GameDataSummaries *game_data_summaries)
{
    kv_destroy(game_data_summaries->summaries);
    free(game_data_summaries);
}

GameDataSummaries *create_game_data_summaries(void)
{
    GameDataSummaries *game_data_summaries = malloc(sizeof(GameDataSummaries));

    kv_init(game_data_summaries->summaries);

    // load json saved data

    game_data_summaries->delete = delete_game_data_summary;
    game_data_summaries->cleanup = cleanup_game_data_summary;

    return game_data_summaries;
}