#include <stdbool.h>
#include <stdlib.h>
#include "raylib.h"
#include "box2d/box2d.h"
#include "ball.h"
#include "entities.h"
#include "resource_manager.h"
#include "settings.h"
#include "game_data.h"
#include "collision_category.h"
#include "b_utils.h"

static float accumlator = 0.0f;

static void set_default_ball_trails(BallTrail *trails, Vector2 position, float radius, Texture2D *texture)
{
    for (int i = 0; i < MAX_TRAIL; i++)
    {
        trails[i].position = vector2_flip_y_center(position, (Vector2){radius * 2, radius * 2});
        trails[i].transparency = 0.2f * (1.0f - (i / (float)(MAX_TRAIL - 1)));
        trails[i].active = true;
        trails[i].texture = texture;
    }
}

void update_ball_trails(Ball *ball)
{
    // Todo: maybe time out the fade and set inactive if faded out to 0;
    // Move existing trails down the array
    for (int i = MAX_TRAIL - 1; i > 0; i--) // Start from the end and move to the front
    {
        ball->balltrails[i].position = ball->balltrails[i - 1].position; // Shift trail down
        ball->balltrails[i].texture = ball->balltrails[i - 1].texture;   // Shift trail down
    }

    b2Vec2 position = b2Body_GetPosition(ball->body);

    ball->balltrails[0].position = vector2_flip_y_center(b2vec2_to_vector2(position), (Vector2){ball->radius * 2, ball->radius * 2});
    ball->balltrails[0].texture = ball->texture;
}

static void clean_up_ball(Ball *ball)
{
    TraceLog(LOG_INFO, "[Destroy] - Box2d Ball [%d] - Success", ball->body.index1);
    b2DestroyBody(ball->body);
    free(ball);
}

static void render_ball(Ball *ball)
{

    b2Vec2 position = b2Body_GetPosition(ball->body);
    // draw larger ball and resize down for subpixel drawing effect
    DrawTextureEx(*ball->texture, vector2_flip_y_center(b2vec2_to_vector2(position), (Vector2){ball->radius * 2, ball->radius * 2}), 0.0f, 0.5f, WHITE);

    // render ball trails
    for (int i = 0; i < MAX_TRAIL; i++)
    {
        // Check if the trail is active before rendering
        if (ball->balltrails[i].active)
        {
            // Set transparency based on trail's transparency value
            Color trail_color = (Color){255, 255, 255, (unsigned char)(ball->balltrails[i].transparency * 255)};

            // Draw the trail texture at its position with the computed transparency
            DrawTextureEx(*ball->balltrails[i].texture, ball->balltrails[i].position, 0.0f, 0.5f, trail_color);
        }
    }
}

static void reset_ball(Ball *ball, b2Vec2 position, b2Vec2 velocity)
{
    ball->active = true;
    b2Body_Enable(ball->body);
    set_default_ball_trails(ball->balltrails, b2vec2_to_vector2(position), ball->radius, ball->texture);
    b2Body_SetTransform(ball->body, position, (b2Rot){0.0f, 1.0f});
    b2Body_SetLinearVelocity(ball->body, velocity);
}

static void disable_ball(Ball *ball)
{
    ball->active = false;
    b2Body_Disable(ball->body);
}

static void update_ball(Ball *ball, float delta_time)
{
    accumlator += delta_time;
    if (accumlator > 0.04)
    {
        update_ball_trails(ball);
        accumlator = 0.0f;
    }

    // Retrieve the current velocity of the ball
    b2Vec2 velocity = b2Body_GetLinearVelocity(ball->body);

    // Cap the horizontal and vertical velocities with max values
    if (fabs(velocity.x) > *ball->max_velocity)
    {
        velocity.x = (velocity.x > 0) ? *ball->max_velocity : -(*ball->max_velocity);
    }

    if (fabs(velocity.y) > *ball->max_velocity)
    {
        velocity.y = (velocity.y > 0) ? *ball->max_velocity : -(*ball->max_velocity);
    }

    if (fabs(velocity.y) < *ball->min_velocity)
    {
        velocity.y = (velocity.y > 0) ? *ball->min_velocity : -(*ball->min_velocity);
    }

    // Set the adjusted velocity back to the ball's body
    b2Body_SetLinearVelocity(ball->body, velocity);
}

Ball *create_ball(GameData *game_data, b2WorldId world_id, b2Vec2 position, b2Vec2 velocity)
{
    Ball *ball = (Ball *)malloc(sizeof(Ball));

    ball->type = ENTITY_BALL;
    ball->active = true;
    ball->texture = &resource_manager.get_texture("ball")->texture;
    ball->radius = ball->texture->width / 4.0f;

    set_default_ball_trails(ball->balltrails, b2vec2_to_vector2(position), ball->radius, ball->texture);

    ball->power = &game_data->ball.power;
    ball->phase_nova = &game_data->perks.phase_shift;
    ball->super_nova = &game_data->perks.super_nova;
    ball->max_velocity = &game_data->ball.max_velocity;
    ball->min_velocity = &game_data->ball.min_velocity;

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
    circle_def.density = 0.003f;
    circle_def.friction = 10.0f;   // Low friction for a smooth ball
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