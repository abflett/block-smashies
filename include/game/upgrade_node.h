#ifndef UPGRADE_NODE_H
#define UPGRADE_NODE_H

#include <stdbool.h>
#include "raylib.h"
#include "resource.h"

typedef enum UpgradeNodeState
{
    NODE_STATE_LOCKED,
    NODE_STATE_UNLOCKED,
    NODE_STATE_PURCHASED
} UpgradeNodeState;

typedef enum UpgradeType
{
    UPGRADE_TYPE_NODE_ATTRIBUTE,
    UPGRADE_TYPE_NODE_PERK
} UpgradeType;

typedef enum UpgradeTypeId
{
    // attributes
    UPGRADE_ORB_POWER = 0,
    UPGRADE_SHIP_FORCE,
    UPGRADE_BOOST_COOLDOWN,
    UPGRADE_BOOST_FORCE,
    UPGRADE_PULSE_COOLDOWN,
    UPGRADE_PULSE_FORCE,
    UPGRADE_MAX_ENERGY,
    UPGRADE_ENERGY_REGEN,
    UPGRADE_MAX_LIVES,
    UPGRADE_MAX_HEAT,
    UPGRADE_HEAT_REGEN,
    UPGRADE_SHIELD_LENGTH,
    UPGRADE_SHIP_FRICTION,
    UPGRADE_SHIP_DAMPING,

    // perks
    UPGRADE_PERK_PHASE_SHIFT,
    UPGRADE_PERK_ORB_SHOT,
    UPGRADE_PERK_SUPER_NOVA,
    UPGRADE_PERK_PHASE_NOVA
} UpgradeTypeId;

typedef struct UpgradeNodeType
{
    UpgradeTypeId type_id;
    UpgradeType type;
    char *name;
    char *description;
    Subtexture *highlight_subtexture;
    Subtexture *icon_subtexture;
} UpgradeNodeType;

typedef struct UpgradeNode
{
    int id; // Unique identifier
    int cost;
    float value;
    Vector2 position; // Position for rendering

    UpgradeNodeType *node_type;
    UpgradeNodeState node_state;

    struct UpgradeNode **prerequisites; // Array pointers of the prerequisites nodes
    int num_prerequisites;              // Number of prerequisites

    Subtexture *background_subtexture;
} UpgradeNode;

#endif // UPGRADE_NODE_H
