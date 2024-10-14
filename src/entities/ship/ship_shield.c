#include "ship_shield.h"

static void set_ship_shield(ShipShield *ship_shield)
{
    int shield_type = 0; // Default to 0 for 1 segment
    if (*ship_shield->segments == 2)
    {
        shield_type = 1; // For 2 segments
    }
    else if (*ship_shield->segments == 3)
    {
        shield_type = 2; // For 4 segments
    }
    else if (*ship_shield->segments == 4)
    {
        shield_type = 2; // For 4 segments
    }

    ship_shield->previous_shield_level = *ship_shield->shield_level;
    ship_shield->previous_segments = *ship_shield->segments;
    const char *subtexture_id = resource_manager.shield_type_mapper->shield_type_to_subtexture_id(shield_type, *ship_shield->shield_level);
    ship_shield->subtexture = resource_manager.get_subtexture(subtexture_id);
}

static void render_ship_shield(ShipShield *ship_shield)
{
    if (ship_shield->previous_shield_level != *ship_shield->shield_level || ship_shield->previous_segments != *ship_shield->segments)
    {
        set_ship_shield(ship_shield);
    }

    float centered_x = (float)(int)(ship_shield->position->x - (ship_shield->subtexture->src.width / 2.0f));
    float offset_y = ship_shield->position->y - 4;
    Vector2 position = (Vector2){centered_x, offset_y};

    DrawTextureRec(ship_shield->subtexture->texture_resource->texture,
                   ship_shield->subtexture->src,
                   position,
                   WHITE);
}

static void cleanup_ship_shield(ShipShield *ship_shield)
{
    free(ship_shield);
}

ShipShield *create_ship_shield(int *segments, int *shield_level, b2Vec2 *position)
{
    ShipShield *ship_shield = malloc(sizeof(ShipShield));

    ship_shield->segments = segments;
    ship_shield->previous_segments = *segments;
    ship_shield->shield_level = shield_level;
    ship_shield->position = position;

    set_ship_shield(ship_shield);
    ship_shield->previous_shield_level = *shield_level;

    // ship_shield->set_shield = set_ship_shield;
    ship_shield->render = render_ship_shield;
    ship_shield->cleanup = cleanup_ship_shield;

    return ship_shield;
}
