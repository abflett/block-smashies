#include <string.h>
#include "node_perk_mapper.h"

static NodePerkMapper mapper;

static const char *node_perk_type_to_subtexture_id(const int node_perk_type)
{
    return kv_A(mapper.subtexture_ids, node_perk_type);
}

static void cleanup(void)
{
    for (size_t i = 0; i < kv_size(mapper.subtexture_ids); i++)
    {
        free(kv_A(mapper.subtexture_ids, i));
    }
    kv_destroy(mapper.subtexture_ids);
}

NodePerkMapper *create_node_perk_mapper(const JSON_Object *root_object)
{
    kv_init(mapper.subtexture_ids);

    const JSON_Object *node_perks_obj = json_object_get_object(root_object, "node-perks");
    const JSON_Array *subtextures_array = json_object_get_array(node_perks_obj, "subtextures");

    for (size_t i = 0; i < json_array_get_count(subtextures_array); i++)
    {
        const char *subtexture_id = json_array_get_string(subtextures_array, i);
        kv_push(char *, mapper.subtexture_ids, strdup(subtexture_id));
    }

    mapper.node_perk_type_to_subtexture_id = node_perk_type_to_subtexture_id;
    mapper.cleanup = cleanup;
    return &mapper;
}
