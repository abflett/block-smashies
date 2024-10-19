#include <stdlib.h>
#include <string.h>
#include "upgrade_manager.h"
#include "parson.h"
#include "settings.h"

#include <stdlib.h>
#include "upgrade_manager.h"

void print_nodes(UpgradeManager *upgrade_manager)
{
    for (int i = 0; i < upgrade_manager->node_count; i++)
    {
        TraceLog(LOG_INFO, "Node %d: %s", i, upgrade_manager->upgrade_nodes[i].name);

        TraceLog(LOG_INFO, "\tPrerequisite");
        for (int j = 0; j < upgrade_manager->upgrade_nodes[i].num_prerequisites; j++)
        {
            TraceLog(LOG_INFO, "\t\t%s", upgrade_manager->upgrade_nodes[i].prerequisites[j]->name);
        }

        TraceLog(LOG_INFO, "\tNext");
        for (int j = 0; j < upgrade_manager->upgrade_nodes[i].num_next; j++)
        {
            TraceLog(LOG_INFO, "\t\t%s", upgrade_manager->upgrade_nodes[i].next[j]->name);
        }
    }
}

// Function to clean up the UpgradeManager
void upgrade_manager_cleanup(UpgradeManager *upgrade_manager)
{
    if (upgrade_manager)
    {
        // Free each upgrade node
        for (int i = 0; i < upgrade_manager->node_count; i++)
        {
            // Free the strings for name and description
            free(upgrade_manager->upgrade_nodes[i].name);
            free(upgrade_manager->upgrade_nodes[i].description);

            // Free the prerequisites array if it was allocated
            free(upgrade_manager->upgrade_nodes[i].prerequisites);

            // Free the next array if it was allocated
            free(upgrade_manager->upgrade_nodes[i].next);
        }

        // Free the array of upgrade nodes
        free(upgrade_manager->upgrade_nodes);

        // Finally, free the UpgradeManager itself
        free(upgrade_manager);
    }
}

UpgradeManager *create_upgrade_manager(GameData *game_data)
{
    UpgradeManager *upgrade_manager = (UpgradeManager *)malloc(sizeof(UpgradeManager));
    upgrade_manager->game_data = game_data;

    JSON_Value *root_value = json_parse_file(settings.file_locations.upgrade_file);
    JSON_Object *root_object = json_value_get_object(root_value);
    JSON_Array *upgrades_array = json_object_get_array(root_object, "upgrades");

    upgrade_manager->node_count = (int)json_array_get_count(upgrades_array);
    upgrade_manager->upgrade_nodes = malloc(sizeof(UpgradeNode) * upgrade_manager->node_count);

    for (int i = 0; i < upgrade_manager->node_count; i++)
    {
        JSON_Object *upgrade_object = json_array_get_object(upgrades_array, i);

        // Populate each node directly
        upgrade_manager->upgrade_nodes[i].id = (int)json_object_get_number(upgrade_object, "id");
        upgrade_manager->upgrade_nodes[i].name = strdup(json_object_get_string(upgrade_object, "name"));
        upgrade_manager->upgrade_nodes[i].description = strdup(json_object_get_string(upgrade_object, "description"));
        upgrade_manager->upgrade_nodes[i].type = (UpgradeType)json_object_get_number(upgrade_object, "type");
        upgrade_manager->upgrade_nodes[i].type_id = (UpgradeTypeId)json_object_get_number(upgrade_object, "type_id");
        upgrade_manager->upgrade_nodes[i].cost = (int)json_object_get_number(upgrade_object, "cost");
        upgrade_manager->upgrade_nodes[i].is_unlocked = false;
        upgrade_manager->upgrade_nodes[i].is_purchased = false;
        upgrade_manager->upgrade_nodes[i].position = (Vector2){0, 0}; // Initialize position
        upgrade_manager->upgrade_nodes[i].texture = NULL;

        // Handle prerequisites
        JSON_Array *prerequisites_array = json_object_get_array(upgrade_object, "prerequisites");
        upgrade_manager->upgrade_nodes[i].num_prerequisites = (int)json_array_get_count(prerequisites_array);
        upgrade_manager->upgrade_nodes[i].prerequisites = malloc(sizeof(UpgradeNode *) * upgrade_manager->upgrade_nodes[i].num_prerequisites);
        for (int j = 0; j < upgrade_manager->upgrade_nodes[i].num_prerequisites; j++)
        {
            int prerequisite_index = (int)json_array_get_number(prerequisites_array, j);
            upgrade_manager->upgrade_nodes[i].prerequisites[j] = &upgrade_manager->upgrade_nodes[prerequisite_index];
        }

        // Handle next nodes
        JSON_Array *next_array = json_object_get_array(upgrade_object, "next");
        upgrade_manager->upgrade_nodes[i].num_next = (int)json_array_get_count(next_array);
        upgrade_manager->upgrade_nodes[i].next = malloc(sizeof(UpgradeNode *) * upgrade_manager->upgrade_nodes[i].num_next);
        for (int j = 0; j < upgrade_manager->upgrade_nodes[i].num_next; j++)
        {
            int next_index = (int)json_array_get_number(next_array, j);
            upgrade_manager->upgrade_nodes[i].next[j] = &upgrade_manager->upgrade_nodes[next_index];
        }
    }

    // Set the current node to the first node (optional)
    upgrade_manager->current_node = &upgrade_manager->upgrade_nodes[0];
    upgrade_manager->current_node->is_unlocked = true; // Unlock the first node

    free(root_value);

    // upgrade_manager->update = upgrade_manager_update;
    // upgrade_manager->render = upgrade_manager_render;
    upgrade_manager->cleanup = upgrade_manager_cleanup;
    upgrade_manager->print_nodes = print_nodes;

    return upgrade_manager;
}
