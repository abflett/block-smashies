#include "collision_manager.h"
#include "raylib.h"
#include "entity_type.h"
#include "ball.h"
#include "brick.h"

static void begin_contact(b2ShapeId shapeA, b2ShapeId shapeB)
{
    b2BodyId bodyA = b2Shape_GetBody(shapeA);
    b2BodyId bodyB = b2Shape_GetBody(shapeB);

    TraceLog(LOG_INFO, "bodyA: %d, bodyB: %d", bodyA.index1, bodyB.index1);

    EntityType *typeA = (EntityType *)b2Body_GetUserData(bodyA);
    EntityType *typeB = (EntityType *)b2Body_GetUserData(bodyB);

    // Ensure the UserData is valid (not NULL)
    if (typeA != NULL && typeB != NULL)
    {
        // Log the EntityType enum values
        TraceLog(LOG_INFO, "typeA: %d, typeB: %d", (int)*typeA, (int)*typeB);
    }
    else
    {
        TraceLog(LOG_ERROR, "UserData is NULL for one or both bodies");
    }

    // // Check if user data is null
    // if (typeA == NULL)
    // {
    //     TraceLog(LOG_ERROR, "Null UserData in begin_contact - typeA");
    // }

    // if (typeB == NULL)
    // {
    //     TraceLog(LOG_ERROR, "Null UserData in begin_contact - typeB");
    // }

    // if (typeA == NULL || typeB == NULL)
    // {
    //     return; // Prevent crash by exiting early
    // }

    // if (*typeA == ENTITY_BALL && *typeB == ENTITY_BRICK)
    // {
    //     Ball *ball = (Ball *)b2Body_GetUserData(bodyA);
    //     Brick *brick = (Brick *)b2Body_GetUserData(bodyB);

    //     // Check if Ball and Brick pointers are valid
    //     if (ball != NULL && brick != NULL)
    //     {
    //         // ball_brick_collision(ball, brick);
    //         TraceLog(LOG_INFO, "Begin Contact - Ball[%d], Brick[%d]", ball->body.index1, brick->body.index1);
    //     }
    // }
    // else if (*typeA == ENTITY_BRICK && *typeB == ENTITY_BALL)
    // {
    //     Ball *ball = (Ball *)b2Body_GetUserData(bodyB);
    //     Brick *brick = (Brick *)b2Body_GetUserData(bodyA);

    //     // Check if Ball and Brick pointers are valid
    //     if (ball != NULL && brick != NULL)
    //     {
    //         // ball_brick_collision(ball, brick);
    //         TraceLog(LOG_INFO, "Begin Contact - Ball[%d], Brick[%d]", ball->body.index1, brick->body.index1);
    //     }
    // }
}

static void end_contact(b2ShapeId shapeA, b2ShapeId shapeB)
{
    // not in use as of yet
}

static void hit_contact(b2ShapeId shapeA, b2ShapeId shapeB, float approachSpeed)
{
    // not in use as of yet
}

static void collision_manager_process_collisions(CollisionManager *collision_manager)
{
    // Get the contact events from the Box2D world
    b2ContactEvents contactEvents = b2World_GetContactEvents(collision_manager->world);

    // Process Begin Touch events
    for (int i = 0; i < contactEvents.beginCount; ++i)
    {
        b2ContactBeginTouchEvent *beginEvent = &contactEvents.beginEvents[i];
        begin_contact(beginEvent->shapeIdA, beginEvent->shapeIdB);
    }

    // Process End Touch events
    for (int i = 0; i < contactEvents.endCount; ++i)
    {
        b2ContactEndTouchEvent *endEvent = &contactEvents.endEvents[i];
        end_contact(endEvent->shapeIdA, endEvent->shapeIdB);
    }

    // Process Hit events
    for (int i = 0; i < contactEvents.hitCount; ++i)
    {
        b2ContactHitEvent *hitEvent = &contactEvents.hitEvents[i];
        hit_contact(hitEvent->shapeIdA, hitEvent->shapeIdB, hitEvent->approachSpeed);
    }
}

CollisionManager create_collision_manager(b2WorldId world_id)
{
    CollisionManager collision_manager;
    collision_manager.world = world_id;
    collision_manager.process_collisions = collision_manager_process_collisions;
    return collision_manager;
}
