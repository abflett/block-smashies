#include <math.h>
#include "paddle.h"
#include "raylib.h"
#include "resource_manager.h"
#include "game_settings.h"
#include "player.h"

#define DEFAULT_SPEED 0.0f
#define ACCELERATION 700.0f
#define MAX_SPEED 300.0f
#define FRICTION 0.95f
#define BOTTOM_PADDING 5

static void handle_player_input(Paddle *paddle, float delta_time, int key_left, int key_right)
{
    if (IsKeyDown(key_left))
    {
        paddle->speed -= *paddle->acceleration * delta_time;
        if (paddle->speed < -*paddle->max_speed)
            paddle->speed = -*paddle->max_speed;
    }

    if (IsKeyDown(key_right))
    {
        paddle->speed += *paddle->acceleration * delta_time;
        if (paddle->speed > *paddle->max_speed)
            paddle->speed = *paddle->max_speed;
    }
}

static void update_paddle(Paddle *paddle, float delta_time)
{
    switch (paddle->player_num)
    {
    case 1:
        handle_player_input(paddle, delta_time, KEY_LEFT, KEY_RIGHT);
        break;

    case 2:
        handle_player_input(paddle, delta_time, KEY_A, KEY_D);
        break;

    // Add more players here as needed
    default:
        break;
    }

    paddle->speed *= *paddle->friction;
    paddle->position.x += paddle->speed * delta_time;

    if (paddle->position.x < game_settings.play_area.x)
    {
        paddle->position.x = game_settings.play_area.x;
    }

    if (paddle->position.x + paddle->size.x > (game_settings.play_area.width + game_settings.play_area.x))
    {
        paddle->position.x = (game_settings.play_area.width + game_settings.play_area.x) - paddle->size.x;
    }
}

static void reset_paddle(Paddle *paddle)
{
    paddle->position = (Vector2){
        ((game_settings.play_area.width) / 2) - (paddle->size.x / 2) + game_settings.play_area.x,
        (game_settings.play_area.height) - paddle->size.y + game_settings.play_area.y - BOTTOM_PADDING};
    paddle->speed = DEFAULT_SPEED;
}

static void render_paddle(Paddle *paddle)
{
    DrawTexture(paddle->texture, (int)paddle->position.x, (int)paddle->position.y, WHITE);
}

static Rectangle get_hitbox_func(Paddle *paddle)
{
    return (Rectangle){paddle->position.x, paddle->position.y, paddle->size.x, paddle->size.y};
}

Paddle create_paddle(int player_num, Player *player)
{
    Paddle paddle;
    paddle.texture = resource_manager.get_texture("paddle")->texture;
    paddle.size = (Vector2){(float)paddle.texture.width, (float)paddle.texture.height};
    paddle.player_num = player_num;
    paddle.position = (Vector2){
        ((game_settings.play_area.width) / 2) - (paddle.size.x / 2) + game_settings.play_area.x,
        (game_settings.play_area.height) - paddle.size.y + game_settings.play_area.y - BOTTOM_PADDING};
    paddle.speed = DEFAULT_SPEED;
    paddle.acceleration = &player->paddle.acceleration;
    paddle.max_speed = &player->paddle.max_speed;
    paddle.friction = &player->paddle.friction;
    paddle.update = update_paddle;
    paddle.reset = reset_paddle;
    paddle.render = render_paddle;
    paddle.get_hitbox = get_hitbox_func;
    paddle.active = true;

    return paddle;
}