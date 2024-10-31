#include <string.h>
#include "upgrade_icon_mapper.h"

static UpgradeIconMapper mapper;

static const char *upgrade_type_to_subtexture_id(const int upgrade_type)
{
    return kv_A(mapper.subtexture_ids, upgrade_type);
}

static void cleanup(void)
{
    for (size_t i = 0; i < kv_size(mapper.subtexture_ids); i++)
    {
        free(kv_A(mapper.subtexture_ids, i));
    }
    kv_destroy(mapper.subtexture_ids);
}

UpgradeIconMapper *create_upgrade_icon_mapper(const JSON_Object *root_object)
{
    kv_init(mapper.subtexture_ids);

    const JSON_Object *upgrade_icons_obj = json_object_get_object(root_object, "upgrade-icons");
    const JSON_Array *subtextures_array = json_object_get_array(upgrade_icons_obj, "subtextures");

    for (size_t i = 0; i < json_array_get_count(subtextures_array); i++)
    {
        const char *subtexture_id = json_array_get_string(subtextures_array, i);
        kv_push(char *, mapper.subtexture_ids, strdup(subtexture_id));
    }

    mapper.upgrade_type_to_subtexture_id = upgrade_type_to_subtexture_id;
    mapper.cleanup = cleanup;
    return &mapper;
}
