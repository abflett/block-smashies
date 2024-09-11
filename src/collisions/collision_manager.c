#include "collision_manager.h"
#include "raylib.h"
#include "entity_type.h"
#include "ball.h"
#include "brick.h"
#include "nanite.h"
// #include "game_context.h"
#include "ball_brick_collision.h"
#include "ball_kill_boundary_collision.h"
#include "nanite_paddle_collision.h"
#include "nanite_kill_boundary_collision.h"

CollisionManager collision_manager;

static void begin_contact(b2ShapeId shapeA, b2ShapeId shapeB, GameContext *context)
{
    b2BodyId bodyA = b2Shape_GetBody(shapeA);
    b2BodyId bodyB = b2Shape_GetBody(shapeB);

    // Get user data once for each body
    void *userDataA = b2Body_GetUserData(bodyA);
    void *userDataB = b2Body_GetUserData(bodyB);

    if (userDataA == NULL || userDataB == NULL)
    {
        TraceLog(LOG_ERROR, "UserData is NULL for one or both bodies");
        return;
    }

    // Cast user data to entity types
    EntityType *typeA = (EntityType *)userDataA;
    EntityType *typeB = (EntityType *)userDataB;

    // Handle ball-brick collision
    if ((*typeA == ENTITY_BALL && *typeB == ENTITY_BRICK) ||
        (*typeA == ENTITY_BRICK && *typeB == ENTITY_BALL))
    {
        Ball *ball = (*typeA == ENTITY_BALL) ? (Ball *)userDataA : (Ball *)userDataB;
        Brick *brick = (*typeA == ENTITY_BRICK) ? (Brick *)userDataA : (Brick *)userDataB;

        if (ball != NULL && brick != NULL)
        {
            ball_brick_collision(ball, brick, context);
        }
    }

    // Handle ball-kill_boundary collision
    if ((*typeA == ENTITY_BALL && *typeB == ENTITY_KILL_BOUNDARY) ||
        (*typeA == ENTITY_KILL_BOUNDARY && *typeB == ENTITY_BALL))
    {
        Ball *ball = (*typeA == ENTITY_BALL) ? (Ball *)userDataA : (Ball *)userDataB;
        KillBoundary *kill_boundary = (*typeA == ENTITY_KILL_BOUNDARY) ? (KillBoundary *)userDataA : (KillBoundary *)userDataB;

        if (ball != NULL && kill_boundary != NULL)
        {
            ball_kill_boundary_collision(ball, kill_boundary, context); // Corrected function call
        }
    }

    if ((*typeA == ENTITY_NANITE && *typeB == ENTITY_PADDLE) ||
        (*typeA == ENTITY_PADDLE && *typeB == ENTITY_NANITE))
    {
        Nanite *nanite = (*typeA == ENTITY_NANITE) ? (Nanite *)userDataA : (Nanite *)userDataB;
        Paddle *paddle = (*typeA == ENTITY_PADDLE) ? (Paddle *)userDataA : (Paddle *)userDataB;

        if (nanite != NULL && paddle != NULL)
        {
            nanite_paddle_collision(nanite, paddle, context); // Corrected function call
        }
    }

    if ((*typeA == ENTITY_NANITE && *typeB == ENTITY_KILL_BOUNDARY) ||
        (*typeA == ENTITY_KILL_BOUNDARY && *typeB == ENTITY_NANITE))
    {
        Nanite *nanite = (*typeA == ENTITY_NANITE) ? (Nanite *)userDataA : (Nanite *)userDataB;
        KillBoundary *kill_boundary = (*typeA == ENTITY_PADDLE) ? (KillBoundary *)userDataA : (KillBoundary *)userDataB;

        if (nanite != NULL && kill_boundary != NULL)
        {
            nanite_kill_boundary_collision(nanite, kill_boundary, context); // Corrected function call
        }
    }
}

// static void end_contact(b2ShapeId shapeA, b2ShapeId shapeB)
// {
//     // not in use as of yet
// }

// static void hit_contact(b2ShapeId shapeA, b2ShapeId shapeB, float approachSpeed)
// {
//     // not in use as of yet
// }

static void collision_manager_process_collisions(GameContext *context)
{
    b2ContactEvents contactEvents = b2World_GetContactEvents(collision_manager.world);

    for (int i = 0; i < contactEvents.beginCount; ++i)
    {
        b2ContactBeginTouchEvent *beginEvent = &contactEvents.beginEvents[i];
        begin_contact(beginEvent->shapeIdA, beginEvent->shapeIdB, context);
    }

    // for (int i = 0; i < contactEvents.endCount; ++i)
    // {
    //     b2ContactEndTouchEvent *endEvent = &contactEvents.endEvents[i];
    //     end_contact(endEvent->shapeIdA, endEvent->shapeIdB);
    // }

    // for (int i = 0; i < contactEvents.hitCount; ++i)
    // {
    //     b2ContactHitEvent *hitEvent = &contactEvents.hitEvents[i];
    //     hit_contact(hitEvent->shapeIdA, hitEvent->shapeIdB, hitEvent->approachSpeed);
    // }
}

CollisionManager *create_collision_manager(b2WorldId world_id)
{
    collision_manager.world = world_id;
    collision_manager.process_collisions = collision_manager_process_collisions;
    return &collision_manager;
}
