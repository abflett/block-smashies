#include <stdlib.h>
#include <string.h>
#include <raylib.h>
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

    JSON_Array *thrusters_array = json_object_get_array(root_object, "thruster-level-animations");

    for (size_t i = 0; i < json_array_get_count(thrusters_array); i++)
    {
        const char *animation_id = json_array_get_string(thrusters_array, i);
        TraceLog(LOG_INFO, "[[[animation id]]] - %s", animation_id);
        kv_push(char *, mapper.animation_ids, strdup(animation_id));
    }

    mapper.thruster_level_to_animation_id = thruster_level_to_animation_id_func;
    mapper.cleanup = cleanup_mapper;
    return &mapper;
}