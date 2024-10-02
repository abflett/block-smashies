#ifndef SHIP_COLOR_MAPPER_H
#define SHIP_COLOR_MAPPER_H

#include "kvec.h"
#include "parson.h"

typedef struct ShipColorMapper
{
    kvec_t(char *) subtexture_ids;

    const char *(*ship_color_to_subtexture_id)(const int ship_color);

    void (*cleanup)(void);
} ShipColorMapper;

ShipColorMapper *create_ship_color_mapper(const JSON_Object *root_object);

#endif