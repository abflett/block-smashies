#include <stdbool.h>
#include <stdlib.h>
#include "paddle.h"
#include "raylib.h"
#include "resource_manager.h"
#include "settings.h"
#include "player.h"
#include "entity_type.h"
#include "collision_category.h"

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
    // timers
    paddle->boost_timer_left += delta_time;
    paddle->boost_timer_right += delta_time;
    paddle->boost_active_timer -= delta_time;
    paddle->pulse_active_timer -= delta_time;
    paddle->pulse_timer -= delta_time;

    // Handle left and right movement
    if (IsKeyDown(KEY_A))
    {
        b2Body_ApplyForceToCenter(paddle->body, (b2Vec2){-*paddle->acceleration, 0.0f}, true);
    }
    if (IsKeyDown(KEY_D))
    {
        b2Body_ApplyForceToCenter(paddle->body, (b2Vec2){*paddle->acceleration, 0.0f}, true);
    }

    // start timer for horizontal boosts
    if (IsKeyReleased(KEY_A))
    {
        paddle->boost_timer_left = 0.0f;
    }
    if (IsKeyReleased(KEY_D))
    {
        paddle->boost_timer_right = 0.0f;
    }

    // apply boost if sucessfully double pressed the left or right and active timer is expired
    if (IsKeyDown(KEY_A) && paddle->boost_timer_left < settings.gameplay.boost_timer && paddle->boost_active_timer <= 0.0f)
    {
        b2Body_ApplyLinearImpulse(paddle->body, (b2Vec2){-*paddle->booster_str, 0.0f}, b2Body_GetWorldCenterOfMass(paddle->body), true);
        paddle->boost_active_timer = settings.gameplay.boost_active_timer;
    }
    if (IsKeyDown(KEY_D) && paddle->boost_timer_right < settings.gameplay.boost_timer && paddle->boost_active_timer <= 0.0f)
    {
        b2Body_ApplyLinearImpulse(paddle->body, (b2Vec2){*paddle->booster_str, 0.0f}, b2Body_GetWorldCenterOfMass(paddle->body), true);
        paddle->boost_active_timer = settings.gameplay.boost_active_timer;
    }

    // Apply upward pulse when up is pressed as long as boost timers are valid
    if (IsKeyPressed(KEY_W) && paddle->pulse_timer <= 0.0f && paddle->pulse_active_timer <= 0)
    {
        b2Body_Disable(paddle->constraint);
        paddle->pulse_timer = settings.gameplay.pulse_timer;
        paddle->pulse_active_timer = settings.gameplay.pulse_active_timer;
        b2Body_ApplyLinearImpulse(paddle->body, (b2Vec2){0.0f, *paddle->pulse_str}, b2Body_GetWorldCenterOfMass(paddle->body), true);
    }

    // return paddle back to X constraint after pulse
    if (paddle->pulse_timer <= 0.0f)
    {
        b2Body_Enable(paddle->constraint);
    }
}

static void reset_paddle(Paddle *paddle, int player_num)
{
    paddle->active = true;
    paddle->player_num = player_num;
    paddle->pulse_timer = 0.0f;
    paddle->pulse_active_timer = 0.0f;
    b2Body_Enable(paddle->body);
    b2Body_SetTransform(paddle->body, (b2Vec2){(settings.game.play_area.width / 2) + settings.game.play_area.x, settings.gameplay.paddle_height}, (b2Rot){1.0f, 0.0f});
}

static void disable_paddle(Paddle *paddle)
{
    paddle->active = false;
    b2Body_Disable(paddle->body);
}

static void render_paddle(Paddle *paddle)
{
    b2Vec2 position = b2Body_GetPosition(paddle->body);
    DrawTextureEx(*paddle->texture, (Vector2){position.x - (paddle->size.x / 2), settings.game.target_size.y - (position.y + (paddle->size.y / 2))}, 0.0f, 1.0f, WHITE);
}

Paddle *create_paddle(int player_num, Player *player, b2WorldId world_id)
{
    Paddle *paddle = (Paddle *)malloc(sizeof(Paddle));
    paddle->type = ENTITY_PADDLE;
    paddle->active = true;

    paddle->texture = &resource_manager.get_texture("ship-base")->texture;
    paddle->size = (b2Vec2){(float)paddle->texture->width, (float)paddle->texture->height};
    paddle->player_num = player_num;
    paddle->pulse_timer = 0.0f;
    paddle->pulse_active_timer = 0.0f;
    paddle->boost_timer_left = 0.0f;
    paddle->boost_timer_right = 0.0f;
    paddle->boost_active_timer = 0.0f;

    // player attribute settings
    paddle->acceleration = &player->paddle.acceleration;
    paddle->max_velocity = &player->paddle.max_velocity;
    paddle->friction = &player->paddle.friction;
    paddle->charge = &player->paddle.charge;
    paddle->booster_str = &player->paddle.booster_str;
    paddle->pulse_str = &player->paddle.pulse_str;
    paddle->phase_shift = &player->perks.phase_shift;
    paddle->time_manipulation = &player->perks.time_manipulation;
    paddle->orb_shot = &player->perks.orb_shot;

    b2BodyDef body_def = b2DefaultBodyDef();
    body_def.type = b2_dynamicBody;
    body_def.position = (b2Vec2){(settings.game.play_area.width / 2) + settings.game.play_area.x, settings.gameplay.paddle_height};
    body_def.linearDamping = 3.0f;
    body_def.isBullet = true;
    paddle->body = b2CreateBody(world_id, &body_def);

    b2Polygon paddle_box = b2MakeBox((paddle->size.x * 0.5f) - 3, paddle->size.y * 0.5f);

    b2ShapeDef paddle_shape_def = b2DefaultShapeDef();
    paddle_shape_def.density = 1.0f;
    paddle_shape_def.friction = 100.0f;
    paddle_shape_def.restitution = 1.0f; // High restitution for bouncing

    paddle_shape_def.filter.categoryBits = CATEGORY_PADDLE;
    paddle_shape_def.filter.maskBits = PADDLE_COLLIDE_WITH;

    b2CreatePolygonShape(paddle->body, &paddle_shape_def, &paddle_box);

    // Create a static reference body for the prismatic joint
    b2BodyDef static_body_def = b2DefaultBodyDef();
    static_body_def.type = b2_staticBody;
    static_body_def.position = (b2Vec2){(settings.game.play_area.width / 2) + settings.game.play_area.x, settings.gameplay.paddle_height}; // Static reference point
    paddle->constraint = b2CreateBody(world_id, &static_body_def);

    // Prismatic Joint: Constrain movement to the X-axis
    b2PrismaticJointDef x_joint_def = b2DefaultPrismaticJointDef();
    x_joint_def.bodyIdA = paddle->body;
    x_joint_def.bodyIdB = paddle->constraint;
    x_joint_def.localAxisA = (b2Vec2){1.0f, 0.0f}; // Constrain to X-axis
    x_joint_def.enableLimit = false;               // No strict limits along X-axis
    x_joint_def.enableMotor = false;
    b2CreatePrismaticJoint(world_id, &x_joint_def);

    b2MassData mass_data = b2Body_GetMassData(paddle->body);
    mass_data.mass = settings.gameplay.paddle_mass;
    b2Body_SetMassData(paddle->body, mass_data);

    paddle->update = update_paddle;
    paddle->render = render_paddle;
    paddle->clean_up = clean_up_paddle;
    paddle->reset = reset_paddle;
    paddle->disable = disable_paddle;

    b2Body_SetUserData(paddle->body, paddle);

    return paddle;
}