#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include "box2d/box2d.h" // For b2WorldId
#include "game_status.h"
#include "entities.h"
#include "game_data.h"
#include "collision_manager.h"
#include "game_ui.h"
#include "shake_effect.h"
#include "level_manager.h"

typedef struct GameContext
{
    Entities *entities;                  // All game entities like balls, paddles, bricks, etc.
    GameData *game_data;                 // GameData object
    b2WorldId world_id;                  // Box2D physics world ID
    CollisionManager *collision_manager; // Manages collisions between entities
    GameStatus *game_status;             // Tracks global game state like time, score, is_paused, etc.
    GameUi *game_ui;
    ShakeEffect *shake_effect;

    LevelManager *level_manager;

    void (*update)(const float delta_time);
    void (*render)(void);
    void (*cleanup)(void);
} GameContext;

// Function declarations for GameContext management (if needed)
GameContext *create_game_context(GameData *game_data);

#endif // GAME_CONTEXT_H