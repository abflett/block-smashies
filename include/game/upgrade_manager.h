#ifndef UPGRADE_MANAGER_H
#define UPGRADE_MANAGER_H

#include "game_data.h"
#include "upgrade_node.h"
#include "raylib.h"
#include "resource.h"
#include "input_manager.h"

typedef struct UpgradeManager
{
    GameData *game_data;
    InputManager *input_manager;
    UpgradeNode *upgrade_nodes;
    int node_count;

    UpgradeNode *current_node;
    Vector2 draw_offset;
    Vector2 camera_offset;
    Subtexture *subtexture_hightlight_perk;
    Subtexture *subtexture_hightlight_attribute;
    float highlight_alpha;
    bool highlight_alpha_increasing;
    Texture *upgrade_display;
    Font *font;
    Subtexture *upgrade_icons[18];

    void (*update)(struct UpgradeManager *upgrade_manager, float delta_time);
    void (*render)(struct UpgradeManager *upgrade_manager);
    void (*cleanup)(struct UpgradeManager *upgrade_manager);
    void (*print_nodes)(struct UpgradeManager *upgrade_manager);
} UpgradeManager;

UpgradeManager *create_upgrade_manager(GameData *game_data);

#endif // UPGRADE_MANAGER_H
