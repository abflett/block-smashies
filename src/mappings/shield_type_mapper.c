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
        ShieldSubtextureIdsSet set = kv_A(mapper.subtexture_sets, i);
        for (int j = 0; j < 4; ++j)
        {
            free(set.subtexture_ids[j]);
        }
    }
    kv_destroy(mapper.subtexture_sets);
}

ShieldTypeMapper *create_shield_type_mapper(const JSON_Object *root_object)
{
    kv_init(mapper.subtexture_sets);

    mapper.shield_type_to_subtexture_id = shield_type_to_subtexture_id_func;
    mapper.cleanup = cleanup_mapper;
    return &mapper;
}
