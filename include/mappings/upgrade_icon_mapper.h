#ifndef UPGRADE_ICON_MAPPER_H
#define UPGRADE_ICON_MAPPER_H

#include "kvec.h"
#include "parson.h"

typedef struct UpgradeIconMapper
{
    kvec_t(char *) subtexture_ids;

    const char *(*upgrade_type_to_subtexture_id)(const int upgrade_type);

    void (*cleanup)(void);
} UpgradeIconMapper;

UpgradeIconMapper *create_upgrade_icon_mapper(const JSON_Object *root_object);

#endif // UPGRADE_ICON_MAPPER_H
