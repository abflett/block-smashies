#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include <stdbool.h>
#include "raylib.h"      // For Texture2D
#include "box2d/box2d.h" // For b2WorldId
#include "game_status.h"
#include "entities.h"
#include "player.h"
#include "collision_manager.h"

typedef struct GameContext
{
    Entities entities;                   // All game entities like balls, paddles, bricks, etc.
    Texture2D *background;               // Background texture for the game
    Player player;                       // Player object
    b2WorldId world_id;                  // Box2D physics world ID
    CollisionManager *collision_manager; // Manages collisions between entities
    GameStatus game_status;              // Tracks global game state like time, score, is_paused, etc.

    void (*update)(float delta_time);
    void (*render)(void);
    void (*cleanup)(void);
} GameContext;

// Function declarations for GameContext management (if needed)
GameContext *create_game_context();

#endif // GAME_CONTEXT_H