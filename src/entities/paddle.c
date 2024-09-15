#include <stdbool.h>
#include <stdlib.h>
#include "paddle.h"
#include "raylib.h"
#include "resource_manager.h"
#include "game_settings.h"
#include "player.h"
#include "entity_type.h"
#include "collision_category.h"

#define MOVE_FORCE 500.0f   // Adjust the force for moving the paddle
#define BOUNCE_FORCE 100.0f // Adjust the force for the bounce
#define MAX_VELOCITY 300.0f // Maximum velocity for the paddle
#define PADDLE_HEIGHT 16.f
#define BOOST_STRENGTH 50.0f

static void clean_up_paddle(Paddle *paddle)
{
    TraceLog(LOG_INFO, "[Cleanup] - Paddle [%d] - Success", paddle->body.index1);
    b2DestroyBody(paddle->body);
    TraceLog(LOG_INFO, "[Cleanup] - Ball [%d] - Success", paddle->constraint.index1);
    b2DestroyBody(paddle->constraint);
    free(paddle);
}

static void update_paddle(Paddle *paddle, float delta_time)
{
    paddle->boost_timer_left += delta_time;
    paddle->boost_timer_right += delta_time;

    // Handle left and right movement
    if (IsKeyDown(KEY_A))
    {
        // Apply a leftward force
        b2Body_ApplyForceToCenter(paddle->body, (b2Vec2){-MOVE_FORCE, 0.0f}, true);
    }
    if (IsKeyDown(KEY_D))
    {
        // Apply a rightward force
        b2Body_ApplyForceToCenter(paddle->body, (b2Vec2){MOVE_FORCE, 0.0f}, true);
    }

    if (IsKeyReleased(KEY_A))
    {
        paddle->boost_timer_left = 0.0f;
    }
    if (IsKeyReleased(KEY_D))
    {
        paddle->boost_timer_right = 0.0f;
    }

    if (paddle->boost_timer_left < 0.3f)
    {
        if (IsKeyDown(KEY_A))
        {
            b2Body_ApplyLinearImpulse(paddle->body, (b2Vec2){-BOOST_STRENGTH, 0.0f}, b2Body_GetWorldCenterOfMass(paddle->body), true);
        }
    }

    if (paddle->boost_timer_right < 0.3f)
    {
        if (IsKeyDown(KEY_D))
        {
            b2Body_ApplyLinearImpulse(paddle->body, (b2Vec2){BOOST_STRENGTH, 0.0f}, b2Body_GetWorldCenterOfMass(paddle->body), true);
        }
    }

    // Apply upward bounce when W is pressed
    if (IsKeyPressed(KEY_W) && paddle->force_timer <= 0.0f && paddle->force_active_timer <= 0)
    {
        b2Body_Disable(paddle->constraint);
        paddle->force_timer = 0.09f;
        paddle->force_active_timer = 2.0f;
        b2Body_ApplyLinearImpulse(paddle->body, (b2Vec2){0.0f, BOUNCE_FORCE}, b2Body_GetWorldCenterOfMass(paddle->body), true);
    }

    if (paddle->force_active_timer > 0.0f)
    {
        paddle->force_active_timer -= delta_time;
    }

    if (paddle->force_timer > 0.0f)
    {
        paddle->force_timer -= delta_time;
    }
    else
    {
        b2Body_Enable(paddle->constraint);
    }
}

static void reset_paddle(Paddle *paddle, int player_num)
{
    paddle->active = true;
    paddle->player_num = player_num;
    paddle->force_timer = 0.0f;
    paddle->force_active_timer = 0.0f;
    b2Body_Enable(paddle->body);
    b2Body_SetTransform(paddle->body, (b2Vec2){(game_settings.play_area.width / 2) + game_settings.play_area.x, PADDLE_HEIGHT}, (b2Rot){1.0f, 0.0f});
}

static void disable_paddle(Paddle *paddle)
{
    paddle->active = false;
    b2Body_Disable(paddle->body);
}

static void render_paddle(Paddle *paddle)
{
    b2Vec2 position = b2Body_GetPosition(paddle->body);
    DrawTextureEx(*paddle->texture, (Vector2){position.x - (paddle->size.x / 2), game_settings.target_height - (position.y + (paddle->size.y / 2))}, 0.0f, 1.0f, WHITE);
}

Paddle *create_paddle(int player_num, Player *player, b2WorldId world_id)
{
    Paddle *paddle = (Paddle *)malloc(sizeof(Paddle));
    paddle->type = ENTITY_PADDLE;
    paddle->active = true;

    paddle->texture = &resource_manager.get_texture("ship-base")->texture;
    paddle->size = (b2Vec2){(float)paddle->texture->width, (float)paddle->texture->height};
    paddle->player_num = player_num;
    paddle->force_timer = 0.0f;
    paddle->force_active_timer = 0.0f;
    paddle->boost_timer_left = 0.0f;
    paddle->boost_timer_right = 0.0f;
    paddle->boost_active_timer = 0.0f;

    // used in later development for power ups but for now ignore
    paddle->acceleration = &player->paddle.acceleration;
    paddle->max_speed = &player->paddle.max_speed;
    paddle->friction = &player->paddle.friction;
    paddle->charge = &player->paddle.charge;
    paddle->booster_str = &player->paddle.booster_str;
    paddle->pulse_str = &player->paddle.pulse_str;
    paddle->phase_shift = &player->perks.phase_shift;
    paddle->time_manipulation = &player->perks.time_manipulation;
    paddle->orb_shot = &player->perks.orb_shot;

    b2BodyDef body_def = b2DefaultBodyDef();
    body_def.type = b2_dynamicBody;
    body_def.position = (b2Vec2){(game_settings.play_area.width / 2) + game_settings.play_area.x, PADDLE_HEIGHT};
    body_def.linearDamping = 3.0f;
    body_def.isBullet = true;
    paddle->body = b2CreateBody(world_id, &body_def);

    b2Polygon paddle_box = b2MakeBox((paddle->size.x * 0.5f) - 3, paddle->size.y * 0.5f);

    b2ShapeDef paddle_shape_def = b2DefaultShapeDef();
    paddle_shape_def.density = 0.004f;
    paddle_shape_def.friction = 100.0f;
    paddle_shape_def.restitution = 1.0f; // High restitution for bouncing

    paddle_shape_def.filter.categoryBits = CATEGORY_PADDLE;
    paddle_shape_def.filter.maskBits = PADDLE_COLLIDE_WITH;

    b2CreatePolygonShape(paddle->body, &paddle_shape_def, &paddle_box);

    // Create a static reference body for the prismatic joint
    b2BodyDef static_body_def = b2DefaultBodyDef();
    static_body_def.type = b2_staticBody;
    static_body_def.position = (b2Vec2){(game_settings.play_area.width / 2) + game_settings.play_area.x, PADDLE_HEIGHT}; // Static reference point
    paddle->constraint = b2CreateBody(world_id, &static_body_def);

    // First Prismatic Joint: Constrain movement to the X-axis
    b2PrismaticJointDef x_joint_def = b2DefaultPrismaticJointDef();
    x_joint_def.bodyIdA = paddle->body;
    x_joint_def.bodyIdB = paddle->constraint;
    x_joint_def.localAxisA = (b2Vec2){1.0f, 0.0f}; // Constrain to X-axis
    x_joint_def.enableLimit = false;               // No strict limits along X-axis
    x_joint_def.enableMotor = false;
    b2CreatePrismaticJoint(world_id, &x_joint_def);

    paddle->update = update_paddle;
    paddle->render = render_paddle;
    paddle->clean_up = clean_up_paddle;
    paddle->reset = reset_paddle;
    paddle->disable = disable_paddle;

    b2Body_SetUserData(paddle->body, paddle);

    return paddle;
}