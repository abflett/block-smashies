#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

struct GameContext;

typedef struct LevelData
{
    int mission;
    int operation;
    float time;
    int data[12 * 12];
} LevelData;

typedef struct LevelManager
{
    struct GameContext *context;
    LevelData level_data;

    void (*load_level)(void);   // loads level_data based on the game_data
    void (*create_level)(void); // adds the entities based on the level_data
} LevelManager;

LevelManager *create_level_manager(struct GameContext *context); // loads level data based the mission and operation in game_data

#endif