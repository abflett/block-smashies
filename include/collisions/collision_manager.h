#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include "box2d/box2d.h"

struct GameContext;

typedef struct CollisionManager
{
    b2WorldId world;
    void (*process_collisions)(struct CollisionManager *collision_manager, struct GameContext *context);
} CollisionManager;

CollisionManager create_collision_manager(b2WorldId world_id);

#endif