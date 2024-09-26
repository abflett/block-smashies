#include <stdbool.h>
#include <stdlib.h>
#include <math.h> // For M_PI
#include "raylib.h"
#include "nanite.h"
#include "settings.h"
#include "game_data.h"
#include "entity_type.h"
#include "collision_category.h"

static void update_nanite(Nanite *nanite, float delta_time)
{
    b2Vec2 velocity = b2Body_GetLinearVelocity(nanite->body);
    float torque = 5.0f; // Adjust the torque value as needed
    b2Body_ApplyTorque(nanite->body, torque, true);

    if (velocity.y < 15)
    {
        b2Body_SetLinearVelocity(nanite->body, (b2Vec2){velocity.x, -settings.gameplay.nanite_y_velocity});
    }
}

static void clean_up_nanite(Nanite *nanite)
{
    TraceLog(LOG_INFO, "[Destroy] - Box2d Nanite [%d] - Success", nanite->body.index1);
    b2DestroyBody(nanite->body);
    free(nanite);
}

static void reset_nanite(Nanite *nanite, b2Vec2 position, float currency, int nanite_type)
{
    nanite->active = true;
    nanite->currency = currency;
    nanite->type = nanite_type;
    const char *subtexture_id = resource_manager.nanite_type_mapper->nanite_type_to_subtexture_id(nanite_type, 0);
    nanite->subtexture = resource_manager.get_subtexture(subtexture_id);
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
    float angular_velocity = b2Body_GetAngularVelocity(nanite->body);
    float rotation_in_degrees = angular_velocity * (180.0f / PI);

    Rectangle rectDest = {position.x, settings.game.target_size.y - position.y, nanite->size.x, nanite->size.y};
    Vector2 origin = {nanite->size.x / 2, nanite->size.y / 2};

    DrawTexturePro(nanite->subtexture->texture_resource->texture, nanite->subtexture->src, rectDest, origin, rotation_in_degrees, WHITE);
}

Nanite *create_nanite(b2WorldId world_id, b2Vec2 position, float currency, int nanite_type)
{
    Nanite *nanite = (Nanite *)malloc(sizeof(Nanite));
    nanite->type = ENTITY_NANITE;
    nanite->active = true;
    nanite->currency = currency;
    nanite->nanite_type = nanite_type;

    const char *subtexture_id = resource_manager.nanite_type_mapper->nanite_type_to_subtexture_id(nanite_type, 0);
    nanite->subtexture = resource_manager.get_subtexture(subtexture_id);

    nanite->size = (b2Vec2){nanite->subtexture->src.width, nanite->subtexture->src.height};

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
    b2Body_SetLinearVelocity(nanite->body, (b2Vec2){0.0f, -settings.gameplay.nanite_y_velocity});

    nanite->update = update_nanite;
    nanite->render = render_nanite;
    nanite->clean_up = clean_up_nanite;
    nanite->reset = reset_nanite;
    nanite->disable = disable_nanite;

    b2Body_SetUserData(nanite->body, nanite);
    b2Body_SetAngularDamping(nanite->body, 0.0f);

    return nanite;
}