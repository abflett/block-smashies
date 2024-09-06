#include <stdbool.h>
#include "raylib.h"
#include "box2d/box2d.h"
#include "ball.h"
#include "entities.h"
#include "resource_manager.h"
#include "game_settings.h"
#include "player.h"

static void update_ball(Ball *ball, Entities *entities, float delta_time)
{
}

static void clean_up_ball(Ball *ball)
{
    b2DestroyBody(ball->body);
}

static void render_ball(Ball *ball)
{

    b2Vec2 position = b2Body_GetPosition(ball->body);
    // draw larger ball and resize down for subpixel drawing effect
    DrawTextureEx(*ball->texture, (Vector2){position.x - ball->radius, position.y + ball->radius}, 0.0f, 0.5f, WHITE);
}

Ball create_ball(Player *player, b2WorldId world_id, b2Vec2 position, b2Vec2 velocity)
{
    Ball ball;
    ball.type = ENTITY_BALL;
    ball.active = true;
    ball.texture = &resource_manager.get_texture("ball")->texture;
    ball.radius = ball.texture->width / 4.0f;

    ball.power = &player->ball.power;
    ball.phase_nova = &player->perks.phase_shift;
    ball.super_nova = &player->perks.super_nova;

    ball.update = update_ball;
    ball.render = render_ball;
    ball.clean_up = clean_up_ball;

    // Create the Box2D body definition
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = position; // Initial position in the Box2D world
    ball.body = b2CreateBody(world_id, &bodyDef);

    // Define the circle shape for the ball
    b2Circle circle;
    circle.center = (b2Vec2){0, 0};             // This is an offset from the body's position
    circle.radius = ball.texture->width / 4.0f; // Texture width is the diameter, so radius is half

    // Define the physical properties of the ball (density, friction, etc.)
    b2ShapeDef circle_def = b2DefaultShapeDef();
    circle_def.density = 1.0f;
    circle_def.friction = 0.0f;    // Low friction for a smooth ball
    circle_def.restitution = 1.0f; // High restitution for bouncing

    // Attach the shape to the body
    b2CreateCircleShape(ball.body, &circle_def, &circle);

    // Apply initial velocity
    b2Body_SetLinearVelocity(ball.body, velocity);

    // Set the user data to associate this Ball with its Box2D body
    b2Body_SetUserData(ball.body, &ball);

    return ball;
}