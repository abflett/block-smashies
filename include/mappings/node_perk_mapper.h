#ifndef NODE_PERK_MAPPER_H
#define NODE_PERK_MAPPER_H

#include "kvec.h"
#include "parson.h"

typedef struct NodePerkMapper
{
    kvec_t(char *) subtexture_ids;

    const char *(*node_perk_type_to_subtexture_id)(const int node_perk_type);

    void (*cleanup)(void);
} NodePerkMapper;

NodePerkMapper *create_node_perk_mapper(const JSON_Object *root_object);

#endif // NODE_PERK_MAPPER_H
