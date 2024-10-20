#ifndef NODE_ATTRIBUTE_MAPPER_H
#define NODE_ATTRIBUTE_MAPPER_H

#include "kvec.h"
#include "parson.h"

typedef struct NodeAttributeMapper
{
    kvec_t(char *) subtexture_ids;

    const char *(*node_attribute_type_to_subtexture_id)(const int node_attribute_type);

    void (*cleanup)(void);
} NodeAttributeMapper;

NodeAttributeMapper *create_node_attribute_mapper(const JSON_Object *root_object);

#endif // NODE_ATTRIBUTE_MAPPER_H
