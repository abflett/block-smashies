#include <stdlib.h>
#include "ball.h"
#include "entities.h"
#include "resource_manager.h"
#include "settings.h"
#include "collision_category.h"
#include "b_utils.h"
#include "raymath.h"

static float accumulator = 0.0f;

static void set_default_ball_trails(BallTrail *trails, b2Vec2 position, float radius, Texture2D *texture)
{
    for (int i = 0; i < MAX_TRAIL; i++)
    {
        const float transparency = 0.2f * (1.0f - ((float)i / (float)(MAX_TRAIL - 1)));

        trails[i].position = position;
        trails[i].color = (Color){255, 255, 255, (int)(transparency * 255)};
        trails[i].active = true;
        trails[i].texture = texture;
    }
}

void update_ball_trails(Ball *ball)
{
    // Move existing trails down the array
    for (int i = MAX_TRAIL - 1; i > 0; i--)
    {
        ball->ball_trails[i].position = ball->ball_trails[i - 1].position;
        ball->ball_trails[i].texture = ball->ball_trails[i - 1].texture;
    }

    ball->ball_trails[0].position = ball->position;
    ball->ball_trails[0].texture = ball->texture;
}

static void clean_up_ball(Ball *ball)
{
    TraceLog(LOG_INFO, "[Destroy] - Box2d Ball [%d] - Success", ball->body.index1);
    b2DestroyBody(ball->body);
    free(ball);
}

static void render_ball(const Ball *ball)
{

    // draw larger ball and resize down for subpixel drawing effect
    render_texture_scale(ball->texture, ball->position, 0.5f);

    // render ball trails
    for (int i = 0; i < MAX_TRAIL; i++)
    {
        // Check if the trail is active before rendering
        if (ball->ball_trails[i].active)
        {
            render_texture_scale_color(ball->ball_trails[i].texture, ball->ball_trails[i].position, 0.5f, ball->ball_trails[i].color);
        }
    }
}

static void reset_ball(Ball *ball, const b2Vec2 position, const b2Vec2 velocity)
{
    ball->active = true;
    b2Body_Enable(ball->body);
    set_default_ball_trails(ball->ball_trails, position, ball->radius, ball->texture);
    b2Body_SetTransform(ball->body, position, (b2Rot){0.0f, 1.0f});
    b2Body_SetLinearVelocity(ball->body, velocity);
}

static void disable_ball(Ball *ball)
{
    ball->active = false;
    b2Body_Disable(ball->body);
}

static void update_ball(Ball *ball, const float delta_time)
{
    accumulator += delta_time;
    if (accumulator > 0.04)
    {
        update_ball_trails(ball);
        accumulator = 0.0f;
    }

    // Retrieve the current velocity of the ball
    ball->position = b2Body_GetPosition(ball->body);
    b2Vec2 velocity = b2Body_GetLinearVelocity(ball->body);

    // double check if ball comes out of bounds
    float left_bound = settings.game.play_area.x;
    float right_bound = settings.game.play_area.x + settings.game.play_area.width;
    float top_bound = settings.game.target_size.y - settings.game.play_area.y; // flip coords

    // Check left boundary
    if (ball->position.x < left_bound)
    {
        ball->position.x = left_bound + 3; // Push ball slightly inside
        b2Body_SetTransform(ball->body, ball->position, (b2Rot){1.0f, 0.0f});
    }

    // Check right boundary
    if (ball->position.x > right_bound)
    {
        ball->position.x = right_bound - 3; // Push ball slightly inside
        b2Body_SetTransform(ball->body, ball->position, (b2Rot){1.0f, 0.0f});
    }

    // Check top boundary
    if (ball->position.y > top_bound)
    {
        ball->position.y = top_bound - 3; // Push ball slightly inside
        b2Body_SetTransform(ball->body, ball->position, (b2Rot){1.0f, 0.0f});
    }

    // Cap the horizontal and vertical velocities with max values
    if (fabsf(velocity.x) > *ball->max_velocity)
    {
        velocity.x = (velocity.x > 0) ? *ball->max_velocity : -(*ball->max_velocity);
    }

    if (fabsf(velocity.y) > *ball->max_velocity)
    {
        velocity.y = (velocity.y > 0) ? *ball->max_velocity : -(*ball->max_velocity);
    }

    if (fabsf(velocity.y) < *ball->min_velocity)
    {
        velocity.y = (velocity.y > 0) ? *ball->min_velocity : -(*ball->min_velocity);
    }

    // Set the adjusted velocity back to the ball's body
    b2Body_SetLinearVelocity(ball->body, velocity);
}

Ball *create_ball(GameData *game_data, const b2WorldId world_id, const b2Vec2 position, const b2Vec2 velocity)
{
    Ball *ball = (Ball *)malloc(sizeof(Ball));

    ball->type = ENTITY_BALL;
    ball->active = true;
    ball->texture = &resource_manager.get_texture("ball")->texture;
    ball->radius = (float)ball->texture->width / 4.0f;
    ball->position = position;

    set_default_ball_trails(ball->ball_trails, ball->position, ball->radius, ball->texture);

    ball->orb_power = &game_data->orb_attributes.orb_power;
    ball->phase_nova = &game_data->perks.phase_shift;
    ball->super_nova = &game_data->perks.super_nova;
    ball->max_velocity = &game_data->orb_attributes.max_velocity;
    ball->min_velocity = &game_data->orb_attributes.min_velocity;

    // Create the Box2D body definition
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = ball->position;
    bodyDef.isBullet = true;
    bodyDef.fixedRotation = true;
    ball->body = b2CreateBody(world_id, &bodyDef);

    // Define the circle shape for the ball
    b2Circle circle;
    circle.center = (b2Vec2){0, 0};               // This is an offset from the body's position
    circle.radius = (float)ball->texture->width * 0.25f; // Texture width is the diameter, so radius is half and scaled half

    // Define the physical properties of the ball (density, friction, etc.)
    b2ShapeDef circle_def = b2DefaultShapeDef();
    circle_def.density = 0.003f;
    circle_def.friction = 0.5f;    // Low friction for a smooth ball
    circle_def.restitution = 1.0f; // High restitution for bouncing

    // Set up the filter to prevent ball-to-ball collisions
    circle_def.filter.categoryBits = CATEGORY_BALL;
    circle_def.filter.maskBits = BALL_COLLIDE_WITH & ~CATEGORY_BALL; // Collide with everything except other balls

    // Attach the shape to the body
    b2CreateCircleShape(ball->body, &circle_def, &circle);
    b2Body_SetLinearVelocity(ball->body, velocity);

    ball->render = render_ball;
    ball->clean_up = clean_up_ball;
    ball->update = update_ball;
    ball->reset = reset_ball;
    ball->disable = disable_ball;

    b2Body_SetUserData(ball->body, ball);
    return ball;
}