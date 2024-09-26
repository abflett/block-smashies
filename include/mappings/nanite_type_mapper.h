#ifndef NANITE_TYPE_MAPPER_H
#define NANITE_TYPE_MAPPER_H

#include "kvec.h"
#include "parson.h"

typedef struct NaniteSubtextureIdsSet
{
    char *subtexture_ids[2]; // array of strings, 4 entries for the set
} NaniteSubtextureIdsSet;

typedef struct NaniteTypeMapper
{
    kvec_t(NaniteSubtextureIdsSet) nanite_sets;
    kvec_t(NaniteSubtextureIdsSet) lg_nanite_sets;

    const char *(*nanite_type_to_subtexture_id)(const int nanite_type, const int index);
    const char *(*lg_nanite_type_to_subtexture_id)(const int nanite_type, const int index);

    void (*cleanup)(void);
} NaniteTypeMapper;

NaniteTypeMapper *create_nanite_type_mapper(const JSON_Object *root_object);

#endif