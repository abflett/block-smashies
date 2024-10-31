#include <stdlib.h>
#include <string.h>
#include "ship_color_mapper.h"

static ShipColorMapper mapper;

static const char *ship_color_to_subtexture_id_func(const int ship_color)
{
    return kv_A(mapper.subtexture_ids, ship_color);
}

static void cleanup_mapper(void)
{
    for (size_t i = 0; i < kv_size(mapper.subtexture_ids); i++)
    {
        free(kv_A(mapper.subtexture_ids, i));
    }
    kv_destroy(mapper.subtexture_ids);
}

ShipColorMapper *create_ship_color_mapper(const JSON_Object *root_object)
{
    kv_init(mapper.subtexture_ids);

    const JSON_Object *ship_colors_obj = json_object_get_object(root_object, "ship-colors");
    const JSON_Array *ship_colors_array = json_object_get_array(ship_colors_obj, "subtextures");

    for (size_t i = 0; i < json_array_get_count(ship_colors_array); i++)
    {
        const char *subtexture_id = json_array_get_string(ship_colors_array, i);
        kv_push(char *, mapper.subtexture_ids, strdup(subtexture_id));
    }

    mapper.ship_color_to_subtexture_id = ship_color_to_subtexture_id_func;
    mapper.cleanup = cleanup_mapper;
    return &mapper;
}
