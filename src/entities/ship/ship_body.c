#include "ship_body.h"
#include "resource_manager.h"
#include "b_utils.h"

static void render_ship_body(ShipBody *ship_body)
{
    const char *subtexture_id = resource_manager.ship_color_mapper->ship_color_to_subtexture_id(*ship_body->ship_color);

    ship_body->subtexture = resource_manager.get_subtexture(subtexture_id);
    DrawTextureRec(ship_body->subtexture->texture_resource->texture, ship_body->subtexture->src, b2vec2_to_vector2(*ship_body->position), WHITE);
}

static void cleanup_ship_body(ShipBody *ship_body)
{
    free(ship_body);
}

ShipBody *create_ship_body(int *segments, int *ship_color, b2Vec2 *position)
{
    ShipBody *ship_body = malloc(sizeof(ShipBody));

    ship_body->segments = segments;
    ship_body->ship_color = ship_color;
    ship_body->position = position;

    const char *subtexture_id = resource_manager.ship_color_mapper->ship_color_to_subtexture_id(*ship_body->ship_color);

    ship_body->subtexture = resource_manager.get_subtexture(subtexture_id);

    ship_body->render = render_ship_body;
    ship_body->cleanup = cleanup_ship_body;
    return ship_body;
}