#include <stdlib.h>
#include "raylib.h"
#include "brick.h"
#include "resource_manager.h"
#include "game_settings.h"
#include "entity_type.h"
#include "entities.h"

#define HEALTH_MODIFIER 1
#define CURRENCY_MODIFIER 1

static char *brick_type_to_subtexture_map[2][4] = {
    {"dk-brown-brick-01", "dk-brown-brick-02", "dk-brown-brick-03", "dk-brown-brick-04"},
    {"brown-brick-01", "brown-brick-02", "brown-brick-03", "brown-brick-04"}};

static const char *brick_type_to_animation_map[2] =
    {"dk-brown-brick-destroy-animation", "brown-brick-destroy-animation"};

static float brick_max_health(int brick_type)
{
    return (float)(brick_type + 1) * HEALTH_MODIFIER;
}

static int brick_currency(Brick *brick)
{
    return (int)(brick->max_health) * CURRENCY_MODIFIER;
}

static void update_brick(Brick *brick, float delta_time)
{
    float health_percentage = brick->health / brick->max_health * 100.0f;
    int subtexture_index = 0;
    if (health_percentage <= 25)
    {
        subtexture_index = 3;
    }
    else if (health_percentage <= 50)
    {
        subtexture_index = 2;
    }
    else if (health_percentage <= 75)
    {
        subtexture_index = 1;
    }

    if (brick->subtexture->id != brick_type_to_subtexture_map[brick->type][subtexture_index])
    {
        brick->subtexture = resource_manager.get_subtexture(brick_type_to_subtexture_map[brick->brick_type][subtexture_index]);
    }

    if (brick->is_destroying)
    {
        brick->animation_handler->update(brick->animation_handler, delta_time);
        if (!brick->animation_handler->is_playing)
        {
            b2Vec2 position = b2Body_GetPosition(brick->body);
            brick->active = false;
            brick->entities->add_nanite(brick->entities, brick->world_id, position, 1);
        }
    }
}

static void clean_up_brick(Brick *brick)
{
    TraceLog(LOG_INFO, "[Cleanup] - Brick [%d] - Success", brick->body.index1);
    b2DestroyBody(brick->body);
    brick->animation_handler->cleanup(brick->animation_handler);
    free(brick);
}

static void render_brick(Brick *brick)
{
    b2Vec2 position = b2Body_GetPosition(brick->body);

    if (brick->animation_handler->is_playing)
    {
        brick->animation_handler->render(brick->animation_handler,
                                         (b2Vec2){position.x - (brick->animation_handler->animation->frames[brick->animation_handler->frame_index].width / 2),
                                                  position.y + ((brick->animation_handler->animation->frames[brick->animation_handler->frame_index].height / 2))});
    }
    else
    {
        DrawTextureRec(brick->subtexture->texture_resource->texture,
                       brick->subtexture->src,
                       (Vector2){position.x - (brick->size.x / 2),
                                 game_settings.target_height - (position.y + (brick->size.y / 2))},
                       WHITE);
    }
}

static void disable_brick(Brick *brick)
{
    brick->is_destroying = true;
    brick->animation_handler->is_playing = true;
    b2Body_Disable(brick->body);
    TraceLog(LOG_INFO, "[Disable] - Brick [%d] disabled.", brick->body.index1);
}

static void reset_brick(Brick *brick, b2Vec2 position, BrickType color)
{
    brick->health = brick_max_health(brick->brick_type);
    brick->max_health = brick_max_health(brick->brick_type);
    brick->active = true;
    b2Body_Enable(brick->body);
    b2Body_SetTransform(brick->body, position, (b2Rot){0.0f, 1.0f});
    TraceLog(LOG_INFO, "[Reset] - Brick [%d] reset to new position and health.", brick->body.index1);
}

Brick *create_brick(Entities *entities, b2WorldId world_id, b2Vec2 position, BrickType brick_type)
{
    Brick *brick = (Brick *)malloc(sizeof(Brick));
    brick->type = ENTITY_BRICK;
    brick->brick_type = brick_type;
    brick->subtexture = resource_manager.get_subtexture(brick_type_to_subtexture_map[brick->brick_type][0]);
    brick->size = (b2Vec2){(float)brick->subtexture->src.width, (float)brick->subtexture->src.height};
    brick->health = brick_max_health(brick->brick_type);
    brick->max_health = brick_max_health(brick->brick_type);
    brick->active = true;
    brick->is_destroying = false;
    brick->animation_handler = create_animation_manager(brick_type_to_animation_map[brick->brick_type], ANIMATION_ONCE, 0.05f);
    brick->animation_handler->is_playing = false;
    brick->entities = entities;
    brick->world_id = world_id;

    b2BodyDef body_def = b2DefaultBodyDef();
    body_def.type = b2_staticBody;
    body_def.position = position;
    brick->body = b2CreateBody(world_id, &body_def);

    b2Polygon brick_box = b2MakeBox(brick->size.x * 0.5f, brick->size.y * 0.5f);

    b2ShapeDef brick_shape_def = b2DefaultShapeDef();
    brick_shape_def.density = 1.0f;
    brick_shape_def.friction = 0.0f;    // Low friction for a smooth ball
    brick_shape_def.restitution = 1.0f; // High restitution for bouncing

    b2CreatePolygonShape(brick->body, &brick_shape_def, &brick_box);

    brick->update = update_brick;
    brick->render = render_brick;
    brick->clean_up = clean_up_brick;
    brick->disable = disable_brick;
    brick->reset = reset_brick;

    b2Body_SetUserData(brick->body, brick);

    return brick;
}
