#ifndef UPGRADE_NODE_H
#define UPGRADE_NODE_H

#include <stdbool.h>
#include "raylib.h"

typedef enum UpgradeNodeState
{
    NODE_STATE_LOCKED,
    NODE_STATE_UNLOCKED,
    NODE_STATE_PURCHASED
} UpgradeNodeState;

typedef enum UpgradeType
{
    ATTRIBUTE,
    PERK
} UpgradeType;

typedef enum UpgradeTypeId
{
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
    UPGRADE_SHIELD_REGEN,
    UPGRADE_SHIP_FRICTION,
    UPGRADE_SHIP_DAMPING,

    UPGRADE_PERK_PHASE_SHIFT = 100,
    UPGRADE_PERK_ORB_SHOT,
    UPGRADE_PERK_SUPER_NOVA,
    UPGRADE_PERK_PHASE_NOVA
} UpgradeTypeId;

typedef struct UpgradeNode
{
    int id; // Unique identifier
    char *name;
    char *description;
    int cost;
    UpgradeType type;                   // attribute or perk
    UpgradeTypeId type_id;              // used for linking upgrades to attributes or perks
    struct UpgradeNode **prerequisites; // Array pointers of the prerequisites nodes
    int num_prerequisites;              // Number of prerequisites
    struct UpgradeNode **next;          // Array pointers of next nodes
    int num_next;                       // Number of next nodes
    UpgradeNodeState node_state;
    Vector2 position;   // Position for rendering
    Texture2D *texture; // Texture for rendering
} UpgradeNode;

#endif // UPGRADE_NODE_H
