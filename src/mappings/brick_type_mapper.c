#include <stdlib.h>
#include <string.h>
#include "brick_type_mapper.h"

static BrickTypeMapper mapper;

static const char *brick_type_to_subtexture_id_func(const int brick_type, const int index)
{
    // Ensure brick_type is non-negative, otherwise default to 0
    size_t brick_type_mod = (brick_type < 0) ? 0 : (brick_type % kv_size(mapper.subtexture_sets));

    // Ensure index is non-negative, otherwise default to 0, and limit it to 0-3
    size_t index_mod = (index < 0) ? 0 : (index % 4);

    // Access the correct subtexture ID using the modded index and brick_type
    return kv_A(mapper.subtexture_sets, brick_type_mod).subtexture_ids[index_mod];
}

static const char *brick_type_to_animation_id_func(const int brick_type)
{
    // Ensure brick_type is non-negative, otherwise default to 0
    size_t brick_type_mod = (brick_type < 0) ? 0 : (brick_type % kv_size(mapper.subtexture_sets));

    return kv_A(mapper.animation_ids, brick_type_mod);
}

static void cleanup_brick_type_mapper(void)
{
    // Free each subtexture ID in the sets
    for (size_t i = 0; i < kv_size(mapper.subtexture_sets); ++i)
    {
        BrickSubtextureIdsSet set = kv_A(mapper.subtexture_sets, i);
        for (int j = 0; j < 4; ++j)
        {
            free(set.subtexture_ids[j]);
        }
    }
    kv_destroy(mapper.subtexture_sets);

    // Free each animation ID
    for (size_t i = 0; i < kv_size(mapper.animation_ids); ++i)
    {
        free(kv_A(mapper.animation_ids, i));
    }
    kv_destroy(mapper.animation_ids);
}

BrickTypeMapper *create_brick_type_mapper(const JSON_Object *root_object)
{
    kv_init(mapper.subtexture_sets);
    kv_init(mapper.animation_ids);

    JSON_Array *brick_type_array = json_object_get_array(root_object, "brick_types");

    for (size_t i = 0; i < json_array_get_count(brick_type_array); i++)
    {
        JSON_Object *brick_type_obj = json_array_get_object(brick_type_array, i);
        const char *animation_id = json_object_get_string(brick_type_obj, "animation");

        JSON_Array *subtexture_array = json_object_get_array(brick_type_obj, "subtextures");
        BrickSubtextureIdsSet subtexture_set = {0};
        for (size_t k = 0; k < json_array_get_count(subtexture_array); k++)
        {
            const char *subtexture_id = json_array_get_string(subtexture_array, k);
            subtexture_set.subtexture_ids[k] = strdup(subtexture_id);
        }
        kv_push(BrickSubtextureIdsSet, mapper.subtexture_sets, subtexture_set);
        kv_push(char *, mapper.animation_ids, strdup(animation_id));
    }

    mapper.brick_type_to_subtexture_id = brick_type_to_subtexture_id_func;
    mapper.brick_type_to_animation_id = brick_type_to_animation_id_func;
    mapper.cleanup = cleanup_brick_type_mapper;

    return &mapper;
}
