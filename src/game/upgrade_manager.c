#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "upgrade_manager.h"
#include "parson.h"
#include "settings.h"

#include <math.h> // Make sure to include math.h for cos and sin

static void update_node_states(UpgradeManager *upgrade_manager)
{
    // Update purchased upgrades based on game_data
    for (int i = 0; i < upgrade_manager->game_data->num_purchased_upgrades; i++)
    {
        upgrade_manager->upgrade_nodes[upgrade_manager->game_data->purchased_upgrades[i]].node_state = NODE_STATE_PURCHASED;
    }

    // Update remaining nodes based on prerequisites
    for (int i = 0; i < upgrade_manager->node_count; i++)
    {
        UpgradeNode *node = &upgrade_manager->upgrade_nodes[i];
        if (node->node_state == upgrade_manager->current_node->node_state || node->node_state == NODE_STATE_PURCHASED)
        {
            continue;
        }

        node->node_state = NODE_STATE_UNLOCKED;
        for (int j = 0; j < node->num_prerequisites; j++)
        {
            if (node->prerequisites[j]->node_state != NODE_STATE_PURCHASED)
            {
                node->node_state = NODE_STATE_LOCKED;
                break;
            }
        }
    }
}

static void update_node_positions(UpgradeManager *upgrade_manager, Vector2 center, float radius_increment)
{
    // Array to store node positions, indexed by layer, group, and node ID
    int layer_groups[24][24][24];
    memset(layer_groups, -1, sizeof(layer_groups)); // Initialize with -1 (assuming node IDs are positive)

    int max_layer = 0;
    int max_group = 0;

    // Manually set root node (layer 0, group 0, node 0)
    int layer = 0;
    int group = 0;
    int node_id = 0;

    layer_groups[layer][group][node_id] = upgrade_manager->upgrade_nodes[0].id;
    upgrade_manager->upgrade_nodes[0].position = center;

    // Loop through all nodes (starting from node 1, since node 0 is root)
    for (int i = 1; i < upgrade_manager->node_count; i++)
    {
        UpgradeNode *node = &upgrade_manager->upgrade_nodes[i];
        int current_node_id = node->id;

        // Step 1: Determine the layer based on prerequisites (distance from the root)
        layer = 0;
        UpgradeNode *checked_node = node;

        // Traverse up the prerequisite chain to find the layer
        while (checked_node != &upgrade_manager->upgrade_nodes[0])
        {
            layer++;
            checked_node = checked_node->prerequisites[0]; // Assuming a single prerequisite for simplicity
        }

        if (layer > max_layer)
        {
            max_layer = layer;
        }

        // Step 2: Determine the group based on prerequisites
        // Nodes with the same prerequisites should belong to the same group.
        // Let's assume the first prerequisite determines the group.
        group = -1; // Reset group each time

        for (int g = 0; g < 24; g++)
        {
            if (layer_groups[layer][g][0] == -1) // If no node in this group
            {
                group = g;
                break;
            }
            UpgradeNode *first_node_in_group = &upgrade_manager->upgrade_nodes[layer_groups[layer][g][0]];
            if (first_node_in_group->prerequisites[0] == node->prerequisites[0]) // Same prerequisite, same group
            {
                group = g;
                break;
            }
        }

        if (group > max_group)
        {
            max_group = group;
        }

        // Step 3: Place the node in the next available position within the group
        node_id = 0;
        while (layer_groups[layer][group][node_id] != -1)
        {
            node_id++; // Find the next available slot in the group
        }
        layer_groups[layer][group][node_id] = current_node_id;

        // TraceLog(LOG_INFO, "Node %d -> Layer %d, Group %d, Node %d", current_node_id, layer, group, node_id);
    }

    for (int layer_count = 0; layer_count <= max_layer; layer_count++)
    {
        for (int group_count = 0; group_count <= max_group; group_count++)
        {
            int nodes_count = 0;
            for (int i = 0; i < 24; i++)
            {
                if (layer_groups[layer_count][group_count][i] != -1)
                {
                    nodes_count++;
                }
            }

            for (int node = 0; node < nodes_count; node++)
            {
                int current_node_id = layer_groups[layer_count][group_count][node];
                UpgradeNode *current_node = &upgrade_manager->upgrade_nodes[current_node_id];
                float radius = radius_increment * layer_count;

                if (layer_count <= 1)
                {

                    float angle = (2 * PI / nodes_count) * node;
                    current_node->position.x = (float)(center.x + radius * cos(angle));
                    current_node->position.y = (float)(center.y + radius * sin(angle));
                }
                else
                {
                    UpgradeNode *prerequisite = current_node->prerequisites[0];
                    float angle_to_prerequisite = (float)atan2(prerequisite->position.y - center.y, prerequisite->position.x - center.x);

                    // Calculate the desired arc length between nodes (e.g., 30 pixels)
                    float desired_arc_length = 30.0f;

                    // Calculate the angle offset based on the desired arc length and current radius
                    float angle_offset = desired_arc_length / radius;

                    // Calculate the total spread based on the number of nodes
                    float total_spread = angle_offset * (nodes_count - 1);

                    // Calculate the starting angle offset to center the spread
                    float start_offset = -total_spread / 2.0f;

                    // Calculate the final angle for this node
                    float node_angle_offset = start_offset + (angle_offset * node);
                    float final_angle = angle_to_prerequisite + node_angle_offset;

                    // Set the node's position based on the final angle around the prerequisite
                    current_node->position.x = (float)(center.x + (radius * cos(final_angle)));
                    current_node->position.y = (float)(center.y + (radius * sin(final_angle)));
                }
            }
        }
    }
}

static Color get_node_color(UpgradeNode *node)
{
    Color color;
    switch (node->node_state)
    {
    case NODE_STATE_LOCKED:
        color = settings.colors.blue_02;
        break;
    case NODE_STATE_UNLOCKED:
        color = settings.colors.blue_04;
        break;
    case NODE_STATE_PURCHASED:
        color = settings.colors.blue_03;
        break;
    }
    return color;
}

static void update(UpgradeManager *upgrade_manager, float delta_time)
{
}

static void render(UpgradeManager *upgrade_manager)
{
    ClearBackground(settings.colors.blue_01);
    // Set the draw offset
    Vector2 offset = upgrade_manager->draw_offset;

    // Draw connections between nodes and their prerequisites, this first so lines are behind nodes
    for (int i = 0; i < upgrade_manager->node_count; i++)
    {
        UpgradeNode *node = &upgrade_manager->upgrade_nodes[i];
        for (int j = 0; j < node->num_prerequisites; j++)
        {
            UpgradeNode *prerequisite = node->prerequisites[j];
            DrawLine((int)(prerequisite->position.x + offset.x), (int)(prerequisite->position.y + offset.y),
                     (int)(node->position.x + offset.x), (int)(node->position.y + offset.y), settings.colors.blue_02);
        }
    }

    // Draw nodes
    for (int i = 0; i < upgrade_manager->node_count; i++)
    {
        UpgradeNode *node = &upgrade_manager->upgrade_nodes[i];

        // Draw the node circle
        Color color = get_node_color(node);
        Vector2 size = (Vector2){20, 20};
        DrawRectangleV((Vector2){node->position.x + offset.x - size.x / 2, node->position.y + offset.y - size.y / 2}, size, color);
        // DrawCircleV((Vector2){node->position.x + offset.x, node->position.y + offset.y}, 10, color);

        // Draw the node ID in the center of the circle
        const char *id_text = TextFormat("%d", node->id);
        int text_width = MeasureText(id_text, 10);                                                                                 // Measure text width to center it
        DrawText(id_text, (int)(node->position.x + offset.x - text_width / 2), (int)(node->position.y + offset.y - 5), 10, BLACK); // Adjust to center the text
    }
}

static void print_nodes(UpgradeManager *upgrade_manager)
{
    for (int i = 0; i < upgrade_manager->node_count; i++)
    {
        TraceLog(LOG_INFO, "Node %d: %s, Position: %f, %f", i, upgrade_manager->upgrade_nodes[i].name, upgrade_manager->upgrade_nodes[i].position.x, upgrade_manager->upgrade_nodes[i].position.y);

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

static void cleanup(UpgradeManager *upgrade_manager)
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
        upgrade_manager->upgrade_nodes[i].node_state = NODE_STATE_LOCKED;
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
    upgrade_manager->current_node = &upgrade_manager->upgrade_nodes[3]; // Todo: set 3 for testing
    upgrade_manager->draw_offset = (Vector2){160, 60};

    free(root_value);

    upgrade_manager->update = update;
    upgrade_manager->render = render;
    upgrade_manager->cleanup = cleanup;
    upgrade_manager->print_nodes = print_nodes;

    update_node_positions(upgrade_manager, (Vector2){0, 0}, 30);
    update_node_states(upgrade_manager);
    return upgrade_manager;
}
