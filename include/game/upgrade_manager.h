#ifndef UPGRADE_MANAGER_H
#define UPGRADE_MANAGER_H

#include "game_data.h"
#include "upgrade_node.h"

typedef struct UpgradeManager
{
    GameData *game_data;
    UpgradeNode *upgrade_nodes;
    int node_count;

    UpgradeNode *current_node;

    void (*update)(struct UpgradeManager *upgrade_manager, float delta_time);
    void (*render)(struct UpgradeManager *upgrade_manager);
    void (*cleanup)(struct UpgradeManager *upgrade_manager);
    void (*print_nodes)(struct UpgradeManager *upgrade_manager);
} UpgradeManager;

UpgradeManager *create_upgrade_manager(GameData *game_data);

#endif // UPGRADE_MANAGER_H
