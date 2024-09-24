#include <stdlib.h>
#include <string.h>
#include "bar_level_mapper.h"

static BarLevelMapper mapper;

static const char *bar_level_to_subtexture_id_func(const int bar_type, const int index)
{
    return kv_A(mapper.bars[bar_type].subtexture_ids, index);
}

static void cleanup_bar_level_mapper(void)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < kv_size(mapper.bars[i].subtexture_ids); j++)
        {
            free(kv_A(mapper.bars[i].subtexture_ids, j)); // Free each string
        }
        kv_destroy(mapper.bars[i].subtexture_ids); // Destroy the kvec
    }
}

BarLevelMapper *create_bar_level_mapper(const JSON_Object *root_object)
{
    JSON_Array *ui_bars_array = json_object_get_array(root_object, "ui-bars");

    for (size_t i = 0; i < json_array_get_count(ui_bars_array) && i < 4; i++)
    {
        kv_init(mapper.bars[i].subtexture_ids);

        JSON_Object *ui_bar_obj = json_array_get_object(ui_bars_array, i);
        JSON_Array *subtexture_array = json_object_get_array(ui_bar_obj, "subtextures");

        mapper.bars[i].count = (int)json_array_get_count(subtexture_array);

        for (int k = 0; k < mapper.bars[i].count; k++)
        {
            const char *subtexture_id = json_array_get_string(subtexture_array, k);
            kv_push(char *, mapper.bars[i].subtexture_ids, strdup(subtexture_id));
        }
    }

    mapper.bar_level_to_subtexture_id = bar_level_to_subtexture_id_func;
    mapper.cleanup = cleanup_bar_level_mapper;

    return &mapper;
}
