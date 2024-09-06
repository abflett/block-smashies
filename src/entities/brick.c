#include "raylib.h"
#include "brick.h"
#include "resource_manager.h"
#include "game_settings.h"
#include "entity_type.h"

static void clean_up_brick(Brick *brick)
{
    TraceLog(LOG_INFO, "[Cleanup] - Brick [%d] - Success", brick->body.index1);
    b2DestroyBody(brick->body);
}

static void render_brick(Brick *brick)
{
    b2Vec2 position = b2Body_GetPosition(brick->body);
    DrawTextureEx(*brick->texture, (Vector2){position.x - (brick->size.x / 2), game_settings.target_height - (position.y + (brick->size.y / 2))}, 0.0f, 1.0f, WHITE);
}

Brick create_brick(b2WorldId world_id, b2Vec2 position, int health)
{
    Brick brick;
    brick.type = ENTITY_BRICK;
    brick.texture = &resource_manager.get_texture("greenbrick")->texture;
    brick.size = (b2Vec2){(float)brick.texture->width, (float)brick.texture->height};
    brick.health = health;
    brick.active = true;

    b2BodyDef body_def = b2DefaultBodyDef();
    body_def.type = b2_staticBody;
    body_def.position = position;
    brick.body = b2CreateBody(world_id, &body_def);

    b2Polygon brick_box = b2MakeBox(brick.size.x * 0.5f, brick.size.y * 0.5f);

    b2ShapeDef brick_shape_def = b2DefaultShapeDef();
    brick_shape_def.density = 1.0f;
    brick_shape_def.friction = 0.0f;    // Low friction for a smooth ball
    brick_shape_def.restitution = 1.0f; // High restitution for bouncing

    b2CreatePolygonShape(brick.body, &brick_shape_def, &brick_box);

    b2Body_SetUserData(brick.body, &brick);

    brick.render = render_brick;
    brick.clean_up = clean_up_brick;

    return brick;
}
