#ifndef BAR_LEVEL_MAPPER_H
#define BAR_LEVEL_MAPPER_H

#include "kvec.h"
#include "parson.h"

typedef struct BarLevelSet
{
    kvec_t(char *) subtexture_ids;
    int count;
} BarLevelSet;

typedef struct BarLevelMapper
{
    BarLevelSet bars[4];
    const char *(*bar_level_to_subtexture_id)(const int bar_type, const int index);
    void (*cleanup)(void);
} BarLevelMapper;

BarLevelMapper *create_bar_level_mapper(const JSON_Object *root_object);

#endif