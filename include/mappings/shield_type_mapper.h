#ifndef SHIELD_TYPE_MAPPER_H
#define SHIELD_TYPE_MAPPER_H

#include "kvec.h"
#include "parson.h"

typedef struct ShieldSubtextureIdsSet
{
    char *subtexture_ids[4]; // array of strings, 4 entries for the set
} ShieldSubtextureIdsSet;

typedef struct ShieldTypeMapper
{
    kvec_t(ShieldSubtextureIdsSet) subtexture_sets;

    const char *(*shield_type_to_subtexture_id)(const int shield_type, const int level);

    void (*cleanup)(void);
} ShieldTypeMapper;

ShieldTypeMapper *create_shield_type_mapper(const JSON_Object *root_object);

#endif