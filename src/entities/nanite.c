#include <stdbool.h>
#include <stdlib.h>
#include <math.h> // For M_PI
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
    float torque = 5.0f; // Adjust the torque value as needed
    b2Body_ApplyTorque(nanite->body, torque, true);

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
    // b2Body_SetAngularVelocity(nanite->body, 0.1f);
}

static void disable_nanite(Nanite *nanite)
{
    nanite->active = false;
    b2Body_Disable(nanite->body);
}

static void render_nanite(Nanite *nanite)
{
    b2Vec2 position = b2Body_GetPosition(nanite->body);
    float angular_velocity = b2Body_GetAngularVelocity(nanite->body);
    float rotation_in_degrees = angular_velocity * (180.0f / PI);

    Rectangle rectSource = {0, 0, nanite->size.x, nanite->size.y};
    Rectangle rectDest = {position.x, game_settings.target_height - position.y, nanite->size.x, nanite->size.y};
    Vector2 origin = {nanite->size.x / 2, nanite->size.y / 2};

    DrawTexturePro(*nanite->texture, rectSource, rectDest, origin, rotation_in_degrees, WHITE);
}

Nanite *create_nanite(b2WorldId world_id, b2Vec2 position, int currency)
{
    Nanite *nanite = (Nanite *)malloc(sizeof(Nanite));
    nanite->type = ENTITY_NANITE;
    nanite->active = true;
    nanite->currency = currency;

    nanite->texture = &resource_manager.get_texture("nanite-01")->texture;
    nanite->size = (b2Vec2){(float)nanite->texture->width, (float)nanite->texture->height};

    b2BodyDef body_def = b2DefaultBodyDef();
    body_def.type = b2_dynamicBody;
    body_def.position = position;
    nanite->body = b2CreateBody(world_id, &body_def);

    b2Polygon nanite_box = b2MakeBox((nanite->size.x * 0.5f), nanite->size.y * 0.5f);

    b2ShapeDef nanite_shape_def = b2DefaultShapeDef();
    nanite_shape_def.density = 0.01f;
    nanite_shape_def.friction = 0.0f;
    nanite_shape_def.restitution = 0.0f; // High restitution for bouncing

    // Set up the filter to prevent ball-to-ball collisions
    nanite_shape_def.filter.categoryBits = CATEGORY_NANITE;
    nanite_shape_def.filter.maskBits = NANITE_COLLIDE_WITH;

    b2CreatePolygonShape(nanite->body, &nanite_shape_def, &nanite_box);

    // Set an initial downward velocity to simulate gravity or make nanites fall
    b2Body_SetLinearVelocity(nanite->body, (b2Vec2){0.0f, -MOVE_FORCE});
    // b2Body_SetAngularVelocity(nanite->body, 0.1f);

    nanite->update = update_nanite;
    nanite->render = render_nanite;
    nanite->clean_up = clean_up_nanite;
    nanite->reset = reset_nanite;
    nanite->disable = disable_nanite;

    b2Body_SetUserData(nanite->body, nanite);
    b2Body_SetAngularDamping(nanite->body, 0.0f);

    return nanite;
}