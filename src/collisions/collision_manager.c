#include "collision_manager.h"
#include "raylib.h"
#include "entity_type.h"
#include "ball.h"
#include "brick.h"
#include "ship.h"
#include "nanite.h"
// #include "game_context.h"
#include "ball_brick_collision.h"
#include "ball_kill_boundary_collision.h"
#include "nanite_ship_collision.h"
#include "nanite_kill_boundary_collision.h"

CollisionManager collision_manager;

static void print_entity_types(const EntityType *type_a, const EntityType *type_b)
{
    const char *a_type;
    const char *b_type;

    switch (*type_a)
    {
    case ENTITY_NONE:
        a_type = "entity_none";
        break;
    case ENTITY_BALL:
        a_type = "entity_ball";
        break;
    case ENTITY_BRICK:
        a_type = "entity_brick";
        break;
    case ENTITY_PADDLE:
        a_type = "entity_paddle";
        break;
    case ENTITY_NANITE:
        a_type = "entity_nanite";
        break;
    case ENTITY_WALL:
        a_type = "entity_wall";
        break;
    case ENTITY_KILL_BOUNDARY:
        a_type = "entity_kill_boundary";
        break;
    default:
        a_type = "unknown";
        break;
    }

    switch (*type_b)
    {
    case ENTITY_NONE:
        b_type = "entity_none";
        break;
    case ENTITY_BALL:
        b_type = "entity_ball";
        break;
    case ENTITY_BRICK:
        b_type = "entity_brick";
        break;
    case ENTITY_PADDLE:
        b_type = "entity_paddle";
        break;
    case ENTITY_NANITE:
        b_type = "entity_nanite";
        break;
    case ENTITY_WALL:
        b_type = "entity_wall";
        break;
    case ENTITY_KILL_BOUNDARY:
        b_type = "entity_kill_boundary";
        break;
    default:
        b_type = "unknown";
        break;
    }

    TraceLog(LOG_INFO, "Collision: (%s), (%s)", a_type, b_type);
}

static void begin_contact(const b2ShapeId shapeA, const b2ShapeId shapeB, const GameContext *context)
{
    const b2BodyId bodyA = b2Shape_GetBody(shapeA);
    const b2BodyId bodyB = b2Shape_GetBody(shapeB);

    // Get user data once for each body
    void *userDataA = b2Body_GetUserData(bodyA);
    void *userDataB = b2Body_GetUserData(bodyB);

    if (userDataA == NULL || userDataB == NULL)
    {
        TraceLog(LOG_ERROR, "UserData is NULL for one or both bodies");
        return;
    }

    // Cast user data to entity types
    const EntityType *typeA = (EntityType *)userDataA;
    const EntityType *typeB = (EntityType *)userDataB;

    print_entity_types(typeA, typeB);

    if ((*typeA == ENTITY_BALL && *typeB == ENTITY_BRICK) ||
        (*typeA == ENTITY_BRICK && *typeB == ENTITY_BALL))
    {
        const Ball *ball = (*typeA == ENTITY_BALL) ? (const Ball *)userDataA : (const Ball *)userDataB;
        Brick *brick = (*typeA == ENTITY_BRICK) ? (Brick *)userDataA : (Brick *)userDataB;

        ball_brick_collision(ball, brick, context);
    }

    // Handle ball-kill_boundary collision
    if ((*typeA == ENTITY_BALL && *typeB == ENTITY_KILL_BOUNDARY) ||
        (*typeA == ENTITY_KILL_BOUNDARY && *typeB == ENTITY_BALL))
    {
        Ball *ball = (*typeA == ENTITY_BALL) ? (Ball *)userDataA : (Ball *)userDataB;
        ball_kill_boundary_collision(ball, context); // Corrected function call
    }

    if ((*typeA == ENTITY_NANITE && *typeB == ENTITY_PADDLE) ||
        (*typeA == ENTITY_PADDLE && *typeB == ENTITY_NANITE))
    {
        Nanite *nanite = (*typeA == ENTITY_NANITE) ? (Nanite *)userDataA : (Nanite *)userDataB;
        nanite_ship_collision(nanite, context); // Corrected function call
    }

    if ((*typeA == ENTITY_NANITE && *typeB == ENTITY_KILL_BOUNDARY) ||
        (*typeA == ENTITY_KILL_BOUNDARY && *typeB == ENTITY_NANITE))
    {
        Nanite *nanite = (*typeA == ENTITY_NANITE) ? (Nanite *)userDataA : (Nanite *)userDataB;
        nanite_kill_boundary_collision(nanite); // Corrected function call
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

static void collision_manager_process_collisions(const GameContext *context)
{
    const b2ContactEvents contactEvents = b2World_GetContactEvents(collision_manager.world);

    for (int i = 0; i < contactEvents.beginCount; ++i)
    {
        const b2ContactBeginTouchEvent *beginEvent = &contactEvents.beginEvents[i];
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

CollisionManager *create_collision_manager(const b2WorldId world_id)
{
    collision_manager.world = world_id;
    collision_manager.process_collisions = collision_manager_process_collisions;
    return &collision_manager;
}
