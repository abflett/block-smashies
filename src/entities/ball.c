#include <stdbool.h>
#include <stdlib.h>
#include "raylib.h"
#include "box2d/box2d.h"
#include "ball.h"
#include "entities.h"
#include "resource_manager.h"
#include "game_settings.h"
#include "player.h"

#define BALL_CATEGORY 0x0002
#define BALL_COLLIDE_WITH 0xFFFF // Collides with everything else except balls
#define MAX_VELOCITY 100.0f

static void clean_up_ball(Ball *ball)
{
    TraceLog(LOG_INFO, "[Cleanup] - Ball [%d] - Success", ball->body.index1);
    b2DestroyBody(ball->body);
    free(ball);
}

static void render_ball(Ball *ball)
{

    b2Vec2 position = b2Body_GetPosition(ball->body);
    // draw larger ball and resize down for subpixel drawing effect
    DrawTextureEx(*ball->texture, (Vector2){position.x - ball->radius, game_settings.target_height - (position.y + ball->radius)}, 0.0f, 0.5f, WHITE);
}

static void update_ball(Ball *ball, float delta_time)
{
    b2Vec2 velocity = b2Body_GetLinearVelocity(ball->body);
    // Cap horizontal velocity
    if (velocity.x > MAX_VELOCITY)
    {
        velocity.x = MAX_VELOCITY;
    }
    else if (velocity.x < -MAX_VELOCITY)
    {
        velocity.x = -MAX_VELOCITY;
    }

    if (velocity.y > MAX_VELOCITY)
    {
        velocity.y = MAX_VELOCITY;
    }
    else if (velocity.y < -MAX_VELOCITY)
    {
        velocity.y = -MAX_VELOCITY;
    }

    b2Body_SetLinearVelocity(ball->body, velocity);
}

Ball *create_ball(Player *player, b2WorldId world_id, b2Vec2 position, b2Vec2 velocity)
{
    Ball *ball = (Ball *)malloc(sizeof(Ball));

    ball->type = ENTITY_BALL;
    ball->active = true;
    ball->texture = &resource_manager.get_texture("ball")->texture;
    ball->radius = ball->texture->width / 4.0f;

    ball->power = &player->ball.power;
    ball->phase_nova = &player->perks.phase_shift;
    ball->super_nova = &player->perks.super_nova;

    // Create the Box2D body definition
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = position; // Initial position in the Box2D world
    bodyDef.isBullet = true;
    ball->body = b2CreateBody(world_id, &bodyDef);

    // Define the circle shape for the ball
    b2Circle circle;
    circle.center = (b2Vec2){0, 0};              // This is an offset from the body's position
    circle.radius = ball->texture->width / 4.0f; // Texture width is the diameter, so radius is half

    // Define the physical properties of the ball (density, friction, etc.)
    b2ShapeDef circle_def = b2DefaultShapeDef();
    circle_def.density = 0.01f;
    circle_def.friction = 10.0f;   // Low friction for a smooth ball
    circle_def.restitution = 1.0f; // High restitution for bouncing

    // Set up the filter to prevent ball-to-ball collisions
    circle_def.filter.categoryBits = BALL_CATEGORY;
    circle_def.filter.maskBits = BALL_COLLIDE_WITH & ~BALL_CATEGORY; // Collide with everything except other balls

    // Attach the shape to the body
    b2CreateCircleShape(ball->body, &circle_def, &circle);
    b2Body_SetLinearVelocity(ball->body, velocity);

    ball->render = render_ball;
    ball->clean_up = clean_up_ball;
    ball->update = update_ball;

    b2Body_SetUserData(ball->body, ball);
    return ball;
}