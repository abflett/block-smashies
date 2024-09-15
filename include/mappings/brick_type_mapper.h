#ifndef BRICK_TYPE_MAPPER
#define BRICK_TYPE_MAPPER

#include "kvec.h"
#include "parson.h"

typedef struct BrickSubtextureIdsSet
{
    char *subtexture_ids[4]; // array of strings, 4 entries for the set
} BrickSubtextureIdsSet;

typedef struct BrickTypeMapper
{
    kvec_t(BrickSubtextureIdsSet) subtexture_sets;
    kvec_t(char *) animation_ids;

    const char *(*brick_type_to_subtexture_id)(const int brick_type, const int index);
    const char *(*brick_type_to_animation_id)(const int brick_type);

    void (*cleanup)(void);
} BrickTypeMapper;

BrickTypeMapper *create_brick_type_mapper(const JSON_Object *root_object);

#endif