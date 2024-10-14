#include "ship_body.h"
#include "resource_manager.h"
#include "b_utils.h"

static void set_color_ship_body(ShipBody *ship_body, int color)
{
    ship_body->previous_ship_color = *ship_body->ship_color;
    const char *subtexture_id = resource_manager.ship_color_mapper->ship_color_to_subtexture_id(*ship_body->ship_color);
    ship_body->subtexture = resource_manager.get_subtexture(subtexture_id);
}

static void render_ship_body(ShipBody *ship_body)
{
    if (*ship_body->ship_color != ship_body->previous_ship_color)
    {
        set_color_ship_body(ship_body, *ship_body->ship_color);
    }

    // Calculate the total width of all segments // Todo: cache these
    float total_width = ship_body->subtexture->src.width * (*ship_body->segments);

    // Offset the position so the segments are centered
    float starting_x = (float)(int)(ship_body->position->x - (total_width / 2.0f));

    for (int i = 0; i < *ship_body->segments; i++)
    {

        Vector2 position = (Vector2){starting_x + (ship_body->subtexture->src.width * i), ship_body->position->y};
        DrawTextureRec(ship_body->subtexture->texture_resource->texture,
                       ship_body->subtexture->src,
                       position,
                       WHITE);
    }
}

static void cleanup_ship_body(ShipBody *ship_body)
{
    free(ship_body);
}

ShipBody *create_ship_body(int *segments, int *ship_color, b2Vec2 *position)
{
    ShipBody *ship_body = malloc(sizeof(ShipBody));
    if (!ship_body)
    {
        log_error("Could not allocate memory for ShipBody");
    }

    ship_body->segments = segments;
    ship_body->ship_color = ship_color;
    ship_body->position = position;

    const char *subtexture_id = resource_manager.ship_color_mapper->ship_color_to_subtexture_id(*ship_body->ship_color);
    ship_body->subtexture = resource_manager.get_subtexture(subtexture_id);

    ship_body->render = render_ship_body;
    ship_body->cleanup = cleanup_ship_body;
    return ship_body;
}