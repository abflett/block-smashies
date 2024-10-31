#include <stdlib.h>
#include <string.h>
#include "nanite_type_mapper.h"

NaniteTypeMapper mapper;

static const char *nanite_type_to_subtexture_id_func(const int nanite_type, const int index)
{
    return kv_A(mapper.nanite_sets, nanite_type).subtexture_ids[index];
}
static const char *lg_nanite_type_to_subtexture_id_func(const int nanite_type, const int index)
{
    return kv_A(mapper.lg_nanite_sets, nanite_type).subtexture_ids[index];
}

static void cleanup_nanite_mapper(void)
{
    for (size_t i = 0; i < kv_size(mapper.nanite_sets); ++i)
    {
        const NaniteSubtextureIdsSet set = kv_A(mapper.nanite_sets, i);
        for (int j = 0; j < 2; ++j)
        {
            free(set.subtexture_ids[j]);
        }
    }
    kv_destroy(mapper.nanite_sets);

    for (size_t i = 0; i < kv_size(mapper.lg_nanite_sets); ++i)
    {
        const NaniteSubtextureIdsSet set = kv_A(mapper.lg_nanite_sets, i);
        for (int j = 0; j < 2; ++j)
        {
            free(set.subtexture_ids[j]);
        }
    }
    kv_destroy(mapper.lg_nanite_sets);
}

NaniteTypeMapper *create_nanite_type_mapper(const JSON_Object *root_object)
{
    kv_init(mapper.nanite_sets);
    kv_init(mapper.lg_nanite_sets);

    const JSON_Array *nanite_type_array = json_object_get_array(root_object, "nanites");
    const JSON_Array *lg_nanite_type_array = json_object_get_array(root_object, "lg-nanites");

    for (size_t i = 0; i < json_array_get_count(nanite_type_array); i++)
    {
        const JSON_Object *nanite_type_obj = json_array_get_object(nanite_type_array, i);

        const JSON_Array *subtexture_array = json_object_get_array(nanite_type_obj, "subtextures");
        NaniteSubtextureIdsSet nanite_subtexture_set = {0};
        for (size_t k = 0; k < json_array_get_count(subtexture_array); k++)
        {
            const char *subtexture_id = json_array_get_string(subtexture_array, k);
            nanite_subtexture_set.subtexture_ids[k] = strdup(subtexture_id);
        }
        kv_push(NaniteSubtextureIdsSet, mapper.nanite_sets, nanite_subtexture_set);
    }

    for (size_t i = 0; i < json_array_get_count(lg_nanite_type_array); i++)
    {
        const JSON_Object *nanite_type_obj = json_array_get_object(lg_nanite_type_array, i);

        const JSON_Array *subtexture_array = json_object_get_array(nanite_type_obj, "subtextures");
        NaniteSubtextureIdsSet nanite_subtexture_set = {0};
        for (size_t k = 0; k < json_array_get_count(subtexture_array); k++)
        {
            const char *subtexture_id = json_array_get_string(subtexture_array, k);
            nanite_subtexture_set.subtexture_ids[k] = strdup(subtexture_id);
        }
        kv_push(NaniteSubtextureIdsSet, mapper.lg_nanite_sets, nanite_subtexture_set);
    }

    mapper.lg_nanite_type_to_subtexture_id = lg_nanite_type_to_subtexture_id_func;
    mapper.nanite_type_to_subtexture_id = nanite_type_to_subtexture_id_func;
    mapper.cleanup = cleanup_nanite_mapper;

    return &mapper;
}
