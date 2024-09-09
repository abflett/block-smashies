#include "collision_manager.h"
#include "raylib.h"
#include "entity_type.h"
#include "ball.h"
#include "brick.h"
#include "game_context.h"
#include "ball_brick_collision.h"

CollisionManager collision_manager;

static void begin_contact(b2ShapeId shapeA, b2ShapeId shapeB, GameContext *context)
{
    b2BodyId bodyA = b2Shape_GetBody(shapeA);
    b2BodyId bodyB = b2Shape_GetBody(shapeB);

    EntityType *typeA = (EntityType *)b2Body_GetUserData(bodyA);
    EntityType *typeB = (EntityType *)b2Body_GetUserData(bodyB);

    if (typeA == NULL || typeB == NULL)
    {
        TraceLog(LOG_ERROR, "UserData is NULL for one or both bodies");
        return;
    }

    // Handle ball-brick collision
    if ((*typeA == ENTITY_BALL && *typeB == ENTITY_BRICK) ||
        (*typeA == ENTITY_BRICK && *typeB == ENTITY_BALL))
    {
        Ball *ball = (*typeA == ENTITY_BALL) ? (Ball *)b2Body_GetUserData(bodyA) : (Ball *)b2Body_GetUserData(bodyB);
        Brick *brick = (*typeA == ENTITY_BRICK) ? (Brick *)b2Body_GetUserData(bodyA) : (Brick *)b2Body_GetUserData(bodyB);

        if (ball != NULL && brick != NULL)
        {
            ball_brick_collision(ball, brick, context);
        }
    }
}

static void end_contact(b2ShapeId shapeA, b2ShapeId shapeB)
{
    // not in use as of yet
}

static void hit_contact(b2ShapeId shapeA, b2ShapeId shapeB, float approachSpeed)
{
    // not in use as of yet
}

static void collision_manager_process_collisions(GameContext *context)
{
    b2ContactEvents contactEvents = b2World_GetContactEvents(collision_manager.world);

    for (int i = 0; i < contactEvents.beginCount; ++i)
    {
        b2ContactBeginTouchEvent *beginEvent = &contactEvents.beginEvents[i];
        begin_contact(beginEvent->shapeIdA, beginEvent->shapeIdB, context);
    }

    for (int i = 0; i < contactEvents.endCount; ++i)
    {
        b2ContactEndTouchEvent *endEvent = &contactEvents.endEvents[i];
        end_contact(endEvent->shapeIdA, endEvent->shapeIdB);
    }

    for (int i = 0; i < contactEvents.hitCount; ++i)
    {
        b2ContactHitEvent *hitEvent = &contactEvents.hitEvents[i];
        hit_contact(hitEvent->shapeIdA, hitEvent->shapeIdB, hitEvent->approachSpeed);
    }
}

CollisionManager *create_collision_manager(b2WorldId world_id)
{
    collision_manager;
    collision_manager.world = world_id;
    collision_manager.process_collisions = collision_manager_process_collisions;
    return &collision_manager;
}
