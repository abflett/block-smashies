#include "thruster_level_mapper.h"

static ThrusterLevelMapper mapper;

static char *thruster_level_to_animation_id_func(const int ship_color)
{
    return kv_A(mapper.animation_ids, ship_color);
}

static void cleanup_mapper(void)
{
    for (size_t i = 0; i < kv_size(mapper.animation_ids); i++)
    {
        free(kv_A(mapper.animation_ids, i)); // Free each string
    }
    kv_destroy(mapper.animation_ids); // Destroy the kvec
}

ThrusterLevelMapper *create_thruster_level_mapper(const JSON_Object *root_object)
{
    kv_init(mapper.animation_ids);

    mapper.thruster_level_to_animation_id = thruster_level_to_animation_id_func;
    mapper.cleanup = cleanup_mapper;
    return &mapper;
}