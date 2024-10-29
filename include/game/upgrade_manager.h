#ifndef UPGRADE_MANAGER_H
#define UPGRADE_MANAGER_H

#include "game_data.h"
#include "upgrade_node.h"
#include "raylib.h"
#include "resource.h"
#include "input_manager.h"
#include "game_dialog.h"

typedef struct UpgradeManager
{
    bool active;

    GameData *game_data;
    InputManager *input_manager;

    UpgradeNode *upgrade_nodes;
    int node_count;

    GameDialog *exit_dialog;

    Subtexture *subtexture_highlight_types[2];
    Subtexture *background_subtexture[2];
    UpgradeNodeType upgrade_node_types[18];

    UpgradeNode *current_node;

    Vector2 draw_offset;
    Vector2 camera_offset;

    float highlight_alpha;
    bool highlight_alpha_increasing;
    Texture *upgrade_display;
    Font *font;

    void (*update)(const struct UpgradeManager *upgrade_manager, const float delta_time);
    void (*render)(const struct UpgradeManager *upgrade_manager);
    void (*cleanup)(struct UpgradeManager *upgrade_manager);
} UpgradeManager;

UpgradeManager *create_upgrade_manager(GameData *game_data);

#endif // UPGRADE_MANAGER_H
