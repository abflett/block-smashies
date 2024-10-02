#ifndef THRUSTER_LEVEL_MAPPER_H
#define THRUSTER_LEVEL_MAPPER_H

#include "kvec.h"
#include "parson.h"

typedef struct ThrusterLevelMapper
{
    kvec_t(char *) animation_ids;

    const char *(*thruster_level_to_animation_id)(const int level);

    void (*cleanup)(void);
} ThrusterLevelMapper;

ThrusterLevelMapper *create_thruster_level_mapper(const JSON_Object *root_object);

#endif