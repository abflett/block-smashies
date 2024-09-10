#include <stdbool.h>
#include <stdlib.h>
#include "raylib.h"
#include "nanite.h"
#include "resource_manager.h"
#include "game_settings.h"
#include "player.h"
#include "entity_type.h"
#include "collision_category.h"

#define MOVE_FORCE 20.0f
// #define MAX_VELOCITY 50.0f

static void update_nanite(Nanite *nanite, float delta_time)
{
    b2Vec2 velocity = b2Body_GetLinearVelocity(nanite->body);

    if (velocity.y < 15)
    {
        b2Body_SetLinearVelocity(nanite->body, (b2Vec2){velocity.x, -MOVE_FORCE});
    }
}

static void clean_up_nanite(Nanite *nanite)
{
    TraceLog(LOG_INFO, "[Cleanup] - Nanite [%d] - Success", nanite->body.index1);
    b2DestroyBody(nanite->body);
    free(nanite);
}

static void reset_nanite(Nanite *nanite, b2Vec2 position, int currency)
{
    nanite->active = true;
    nanite->currency = currency;
    b2Body_Enable(nanite->body);
    b2Body_SetTransform(nanite->body, position, (b2Rot){1.0f, 0.0f});
}

static void disable_nanite(Nanite *nanite)
{
    nanite->active = false;
    b2Body_Disable(nanite->body);
}

static void render_nanite(Nanite *nanite)
{
    b2Vec2 position = b2Body_GetPosition(nanite->body);
    DrawTextureEx(*nanite->texture, (Vector2){position.x - (nanite->size.x / 2), game_settings.target_height - (position.y + (nanite->size.y / 2))}, 0.0f, 1.0f, WHITE);
}

Nanite *create_nanite(b2WorldId world_id, b2Vec2 position, int currency)
{
    Nanite *nanite = (Nanite *)malloc(sizeof(Nanite));
    nanite->type = ENTITY_NANITE;
    nanite->active = true;
    nanite->currency = currency;

    nanite->texture = &resource_manager.get_texture("nanite001")->texture;
    nanite->size = (b2Vec2){(float)nanite->texture->width, (float)nanite->texture->height};

    b2BodyDef body_def = b2DefaultBodyDef();
    body_def.type = b2_dynamicBody;
    body_def.position = position;
    nanite->body = b2CreateBody(world_id, &body_def);

    b2Polygon nanite_box = b2MakeBox((nanite->size.x * 0.5f), nanite->size.y * 0.5f);

    b2ShapeDef nanite_shape_def = b2DefaultShapeDef();
    nanite_shape_def.density = 0.004f;
    nanite_shape_def.friction = 0.0f;
    nanite_shape_def.restitution = 1.0f; // High restitution for bouncing

    // Set up the filter to prevent ball-to-ball collisions
    nanite_shape_def.filter.categoryBits = CATEGORY_NANITE;
    nanite_shape_def.filter.maskBits = NANITE_COLLIDE_WITH;

    b2CreatePolygonShape(nanite->body, &nanite_shape_def, &nanite_box);

    // Set an initial downward velocity to simulate gravity or make nanites fall
    b2Body_SetLinearVelocity(nanite->body, (b2Vec2){0.0f, -MOVE_FORCE});

    nanite->update = update_nanite;
    nanite->render = render_nanite;
    nanite->clean_up = clean_up_nanite;
    nanite->reset = reset_nanite;
    nanite->disable = disable_nanite;

    b2Body_SetUserData(nanite->body, nanite);

    return nanite;
}