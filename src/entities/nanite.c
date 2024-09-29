#include <stdbool.h>
#include <stdlib.h>
#include <math.h> // For M_PI
#include "raylib.h"
#include "nanite.h"
#include "settings.h"
#include "game_data.h"
#include "entity_type.h"
#include "collision_category.h"
#include "b_utils.h"

static void update_nanite(Nanite *nanite, float delta_time)
{
    nanite->current_rotation += delta_time * nanite->rotation_speed;
    nanite->current_rotation = fmodf(nanite->current_rotation, 360.0f);

    int subtexture_index = 0;
    if ((nanite->current_rotation >= 0 && nanite->current_rotation < 30) ||
        (nanite->current_rotation >= 90 && nanite->current_rotation < 120))
    {
        subtexture_index = 1; // Sparkly texture
    }

    const char *subtexture_id;
    if (nanite->large_nanite)
    {
        subtexture_id = resource_manager.nanite_type_mapper->lg_nanite_type_to_subtexture_id(nanite->nanite_type, subtexture_index);
    }
    else
    {
        subtexture_id = resource_manager.nanite_type_mapper->nanite_type_to_subtexture_id(nanite->nanite_type, subtexture_index);
    }
    nanite->subtexture = resource_manager.get_subtexture(subtexture_id);
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

    nanite->nanite_type = nanite_type;

    int large_chance = GetRandomValue(0, 100);
    const char *subtexture_id;
    if (large_chance > 95)
    {
        nanite->large_nanite = true;
        subtexture_id = resource_manager.nanite_type_mapper->lg_nanite_type_to_subtexture_id(nanite_type, 0);
        nanite->currency = currency * 2;
    }
    else
    {
        nanite->large_nanite = false;
        subtexture_id = resource_manager.nanite_type_mapper->nanite_type_to_subtexture_id(nanite_type, 0);
        nanite->currency = currency;
    }
    nanite->subtexture = resource_manager.get_subtexture(subtexture_id);

    nanite->size = (b2Vec2){nanite->subtexture->src.width, nanite->subtexture->src.height};
    nanite->rotation_speed = (float)GetRandomValue(90, 360);
    b2Polygon nanite_box = b2MakeBox(nanite->size.x * 0.5f, nanite->size.y * 0.5f);
    b2Body_Enable(nanite->body);
    b2Shape_SetPolygon(nanite->shape_id, &nanite_box);
    b2Body_SetTransform(nanite->body, position, (b2Rot){1.0f, 0.0f});
    b2Body_SetLinearVelocity(nanite->body, (b2Vec2){0.0f, -settings.gameplay.nanite_y_velocity});
}

static void disable_nanite(Nanite *nanite)
{
    nanite->active = false;
    b2Body_Disable(nanite->body);
}

static void render_nanite(Nanite *nanite)
{
    b2Vec2 position = b2Body_GetPosition(nanite->body);

    Rectangle rectDest = {position.x, flip_y(position.y), nanite->size.x, nanite->size.y};
    Vector2 origin = {nanite->size.x * 0.5f, nanite->size.y * 0.5f};
    DrawTexturePro(nanite->subtexture->texture_resource->texture, nanite->subtexture->src, rectDest, origin, nanite->current_rotation, WHITE);
}

Nanite *create_nanite(b2WorldId world_id, b2Vec2 position, float currency, int nanite_type)
{
    Nanite *nanite = (Nanite *)malloc(sizeof(Nanite));
    nanite->type = ENTITY_NANITE;
    nanite->active = true;

    nanite->nanite_type = nanite_type;

    int large_chance = GetRandomValue(1, 100);
    const char *subtexture_id;
    if (large_chance > 95)
    {
        nanite->large_nanite = true;
        subtexture_id = resource_manager.nanite_type_mapper->lg_nanite_type_to_subtexture_id(nanite_type, 0);
        nanite->currency = currency * 2;
    }
    else
    {
        nanite->large_nanite = false;
        subtexture_id = resource_manager.nanite_type_mapper->nanite_type_to_subtexture_id(nanite_type, 0);
        nanite->currency = currency;
    }
    nanite->subtexture = resource_manager.get_subtexture(subtexture_id);

    nanite->current_rotation = (float)GetRandomValue(1, 359);
    nanite->rotation_speed = (float)GetRandomValue(90, 360);
    nanite->size = (b2Vec2){nanite->subtexture->src.width, nanite->subtexture->src.height};

    b2BodyDef body_def = b2DefaultBodyDef();
    body_def.type = b2_dynamicBody;
    body_def.position = position;
    nanite->body = b2CreateBody(world_id, &body_def);

    b2Polygon nanite_box = b2MakeBox((nanite->size.x * 0.5f), nanite->size.y * 0.5f);

    b2ShapeDef nanite_shape_def = b2DefaultShapeDef();
    nanite_shape_def.density = 0.01f;
    nanite_shape_def.friction = 0.0f;
    nanite_shape_def.restitution = 0.0f;

    nanite_shape_def.filter.categoryBits = CATEGORY_NANITE;
    nanite_shape_def.filter.maskBits = NANITE_COLLIDE_WITH;

    nanite->shape_id = b2CreatePolygonShape(nanite->body, &nanite_shape_def, &nanite_box);

    // Set an initial downward velocity to simulate gravity or make nanites fall
    b2Body_SetLinearVelocity(nanite->body, (b2Vec2){0.0f, -settings.gameplay.nanite_y_velocity});

    nanite->update = update_nanite;
    nanite->render = render_nanite;
    nanite->clean_up = clean_up_nanite;
    nanite->reset = reset_nanite;
    nanite->disable = disable_nanite;

    b2Body_SetUserData(nanite->body, nanite);
    return nanite;
}