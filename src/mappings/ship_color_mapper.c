#include "ship_color_mapper.h"

static ShipColorMapper mapper;

static char *ship_color_to_subtexture_id_func(const int ship_color)
{
    return kv_A(mapper.subtexture_ids, ship_color);
}

static void cleanup_mapper(void)
{
    for (size_t i = 0; i < kv_size(mapper.subtexture_ids); i++)
    {
        free(kv_A(mapper.subtexture_ids, i)); // Free each string
    }
    kv_destroy(mapper.subtexture_ids); // Destroy the kvec
}

ShipColorMapper *create_ship_color_mapper(const JSON_Object *root_object)
{
    kv_init(mapper.subtexture_ids);

    mapper.ship_color_to_subtexture_id = ship_color_to_subtexture_id_func;
    mapper.cleanup = cleanup_mapper;
    return &mapper;
}
