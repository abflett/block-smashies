#include <stdlib.h>
#include <string.h>
#include "shield_type_mapper.h"

static ShieldTypeMapper mapper;

static const char *shield_type_to_subtexture_id_func(const int shield_type, const int level)
{
    return kv_A(mapper.subtexture_sets, shield_type).subtexture_ids[level];
}

static void cleanup_mapper(void)
{
    for (size_t i = 0; i < kv_size(mapper.subtexture_sets); ++i)
    {
        const ShieldSubtextureIdsSet *set = &kv_A(mapper.subtexture_sets, i);
        for (int j = 0; j < 4; ++j)
        {
            free(set->subtexture_ids[j]);
        }
    }
    kv_destroy(mapper.subtexture_sets);
}

ShieldTypeMapper *create_shield_type_mapper(const JSON_Object *root_object)
{
    kv_init(mapper.subtexture_sets);

    const JSON_Array *shield_type_array = json_object_get_array(root_object, "shield-types");

    for (size_t i = 0; i < json_array_get_count(shield_type_array); i++)
    {
        const JSON_Object *shield_type_obj = json_array_get_object(shield_type_array, i);

        const JSON_Array *subtexture_array = json_object_get_array(shield_type_obj, "subtextures");
        ShieldSubtextureIdsSet subtexture_set = {0};
        for (size_t k = 0; k < json_array_get_count(subtexture_array); k++)
        {
            const char *subtexture_id = json_array_get_string(subtexture_array, k);
            subtexture_set.subtexture_ids[k] = strdup(subtexture_id);
        }
        kv_push(ShieldSubtextureIdsSet, mapper.subtexture_sets, subtexture_set);
    }

    mapper.shield_type_to_subtexture_id = shield_type_to_subtexture_id_func;
    mapper.cleanup = cleanup_mapper;
    return &mapper;
}
