#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "upgrade_manager.h"
#include "parson.h"
#include "settings.h"
#include "resource_manager.h"
#include "raymath.h"

#include <math.h> // Make sure to include math.h for cos and sin

static void update_node_states(UpgradeManager *upgrade_manager)
{

    // Update purchased upgrades based on game_data
    for (int i = 0; i < upgrade_manager->game_data->num_purchased_upgrades; i++)
    {
        UpgradeNode *upgrade_node = &upgrade_manager->upgrade_nodes[upgrade_manager->game_data->purchased_upgrades[i]];
        upgrade_node->node_state = NODE_STATE_PURCHASED;
        UpgradeType upgrade_type = upgrade_node->type;

        upgrade_node->subtexture = upgrade_type == UPGRADE_TYPE_NODE_PERK ? resource_manager.get_subtexture(resource_manager.node_perk_mapper->node_perk_type_to_subtexture_id((int)NODE_STATE_PURCHASED)) : resource_manager.get_subtexture(resource_manager.node_attribute_mapper->node_attribute_type_to_subtexture_id((int)NODE_STATE_PURCHASED));
    }

    // Update remaining nodes based on prerequisites
    for (int i = 0; i < upgrade_manager->node_count; i++)
    {
        UpgradeNode *node = &upgrade_manager->upgrade_nodes[i];
        Subtexture *subtexture_perk = resource_manager.get_subtexture(resource_manager.node_perk_mapper->node_perk_type_to_subtexture_id((int)NODE_STATE_LOCKED));
        Subtexture *subtexture_attribute = resource_manager.get_subtexture(resource_manager.node_attribute_mapper->node_attribute_type_to_subtexture_id((int)NODE_STATE_LOCKED));

        if (node->node_state == NODE_STATE_PURCHASED)
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

        UpgradeType upgrade_type = node->type;
        node->subtexture = upgrade_type == UPGRADE_TYPE_NODE_PERK ? resource_manager.get_subtexture(resource_manager.node_perk_mapper->node_perk_type_to_subtexture_id((int)node->node_state)) : resource_manager.get_subtexture(resource_manager.node_attribute_mapper->node_attribute_type_to_subtexture_id((int)node->node_state));
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
                    float desired_arc_length = radius_increment;

                    // Calculate the angle offset based on the desired arc length and current radius
                    float angle_offset = desired_arc_length / radius;

                    // Calculate the total spread based on the number of nodes
                    float total_spread = angle_offset * (nodes_count - 1);

                    // Calculate the starting angle offset to center the spread
                    float start_offset = -total_spread * 0.5f;

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
    Vector2 temp_position = upgrade_manager->current_node->position;

    bool moved = false;
    for (int player = 0; player < upgrade_manager->game_data->player_count; player++)
    {
        InputManager *input_manager = upgrade_manager->input_manager;
        InputMapping *input = input_manager->get_player_input(player);
        int mapping = input_manager->player[player];

        if (input_manager->key_debounce(mapping, input->action_k_UP))
        {
            temp_position.y -= 15;
            moved = true;
            break;
        }
        if (input_manager->key_debounce(mapping, input->action_k_DOWN))
        {
            temp_position.y += 15;
            moved = true;
            break;
        }
        if (input_manager->key_debounce(mapping, input->action_k_LEFT))
        {
            temp_position.x -= 15;
            moved = true;
            break;
        }
        if (input_manager->key_debounce(mapping, input->action_k_RIGHT))
        {
            temp_position.x += 15;
            moved = true;
            break;
        }
    }

    if (moved)
    {
        // set and move to closest node
        UpgradeNode *closest_node = upgrade_manager->current_node;
        float closest_distance = 99999999.99f;
        for (int i = 0; i < upgrade_manager->node_count; i++)
        {
            UpgradeNode *working_node = &upgrade_manager->upgrade_nodes[i];
            float working_distance = Vector2Distance(temp_position, upgrade_manager->upgrade_nodes[i].position);

            if (working_distance < closest_distance && upgrade_manager->current_node != working_node)
            {
                closest_distance = working_distance;
                closest_node = working_node;
            }
        }
        upgrade_manager->current_node = closest_node;

        // move camera based on new current node
        Vector2 center_position = upgrade_manager->upgrade_nodes[0].position;
        upgrade_manager->camera_offset = (Vector2){upgrade_manager->current_node->position.x - center_position.x, upgrade_manager->current_node->position.y - center_position.y};
    }
}

static void display_details(UpgradeManager *upgrade_manager)
{
    UpgradeNode *node = upgrade_manager->current_node;

    const char *cost_text;
    switch (node->node_state)
    {
    case NODE_STATE_LOCKED:
        cost_text = "Locked";
        break;
    case NODE_STATE_UNLOCKED:
        cost_text = TextFormat("Cost: %d", node->cost);
        break;
    case NODE_STATE_PURCHASED:
        cost_text = "Purchased";
        break;
    default:
        break;
    }

    const char *node_type_text = TextFormat("%s: %s", node->type == UPGRADE_TYPE_NODE_PERK ? "Perk" : "Attribute", node->name);
    DrawTextEx(*upgrade_manager->font, node_type_text, (Vector2){44, 136}, 7, 0.0f, settings.colors.blue_04);

    DrawTextEx(*upgrade_manager->font, node->description, (Vector2){44, 151}, 7, 0.0f, settings.colors.blue_04);

    Vector2 cost_text_size = MeasureTextEx(*upgrade_manager->font, cost_text, 7, 0.0f);
    DrawTextEx(*upgrade_manager->font, cost_text, (Vector2){277 - cost_text_size.x, 136}, 7, 0.0f, settings.colors.blue_04);

    const char *node_id_text = TextFormat("Id: %d", node->id);
    Vector2 id_text_size = MeasureTextEx(*upgrade_manager->font, node_id_text, 7, 0.0f);
    DrawTextEx(*upgrade_manager->font, node_id_text, (Vector2){277 - id_text_size.x, 161}, 7, 0.0f, settings.colors.blue_04);
}

static void render(UpgradeManager *upgrade_manager)
{
    ClearBackground(settings.colors.blue_01);
    // Set the draw offset
    Vector2 offset = Vector2Subtract(upgrade_manager->draw_offset, upgrade_manager->camera_offset);

    // Draw connections between nodes and their prerequisites, this first so lines are behind nodes
    for (int i = 0; i < upgrade_manager->node_count; i++)
    {
        UpgradeNode *node = &upgrade_manager->upgrade_nodes[i];
        for (int j = 0; j < node->num_prerequisites; j++)
        {
            UpgradeNode *prerequisite = node->prerequisites[j];

            DrawLineEx((Vector2){(prerequisite->position.x + offset.x), (prerequisite->position.y + offset.y)},
                       (Vector2){(node->position.x + offset.x), (node->position.y + offset.y)}, 2.0f, settings.colors.blue_02);
        }
    }

    // Draw nodes
    for (int i = 0; i < upgrade_manager->node_count; i++)
    {
        UpgradeNode *node = &upgrade_manager->upgrade_nodes[i];
        // draw node highlight
        if (node == upgrade_manager->current_node)
        {
            Subtexture *subtexture_highlight = node->type == UPGRADE_TYPE_NODE_PERK ? upgrade_manager->subtexture_hightlight_perk : upgrade_manager->subtexture_hightlight_attribute;
            Vector2 highlight_size = (Vector2){subtexture_highlight->src.width, subtexture_highlight->src.height};
            DrawTexturePro(subtexture_highlight->texture_resource->texture, subtexture_highlight->src, (Rectangle){node->position.x + offset.x - highlight_size.x * 0.5f + 1, node->position.y + offset.y - highlight_size.y * 0.5f, highlight_size.x, highlight_size.y}, (Vector2){1, 0}, 0, WHITE);
        }

        // draw node body
        Vector2 size = (Vector2){node->subtexture->src.width, node->subtexture->src.height};
        DrawTexturePro(node->subtexture->texture_resource->texture, node->subtexture->src, (Rectangle){node->position.x + offset.x - size.x * 0.5f + 1, node->position.y + offset.y - size.y * 0.5f, size.x, size.y}, (Vector2){1, 0}, 0, WHITE);

        // Draw the node ID in the center of the circle
        const char *id_text = TextFormat("%d", node->id);
        Vector2 id_text_size = MeasureTextEx(*upgrade_manager->font, id_text, 7, 0.0f);
        DrawTextEx(*upgrade_manager->font, id_text, (Vector2){(float)(int)(node->position.x + offset.x - id_text_size.x * 0.5f + 1), (float)(int)(node->position.y + offset.y - id_text_size.y * 0.5f)}, 7, 0.0f, settings.colors.blue_05); // Measure text width to center it
    }

    // Draw the upgrade display
    DrawTexture(*upgrade_manager->upgrade_display, 0, 0, WHITE);

    display_details(upgrade_manager);
}

// Todo: remove debugging printing function
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

    // default locked textures
    Subtexture *subtexture_perk = resource_manager.get_subtexture(resource_manager.node_perk_mapper->node_perk_type_to_subtexture_id((int)NODE_STATE_LOCKED));
    Subtexture *subtexture_attribute = resource_manager.get_subtexture(resource_manager.node_attribute_mapper->node_attribute_type_to_subtexture_id((int)NODE_STATE_LOCKED));

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
        upgrade_manager->upgrade_nodes[i].subtexture = upgrade_manager->upgrade_nodes[i].type == UPGRADE_TYPE_NODE_PERK ? subtexture_perk : subtexture_attribute;

        // assign prerequisites
        JSON_Array *prerequisites_array = json_object_get_array(upgrade_object, "prerequisites");
        upgrade_manager->upgrade_nodes[i].num_prerequisites = (int)json_array_get_count(prerequisites_array);
        upgrade_manager->upgrade_nodes[i].prerequisites = malloc(sizeof(UpgradeNode *) * upgrade_manager->upgrade_nodes[i].num_prerequisites);
        for (int j = 0; j < upgrade_manager->upgrade_nodes[i].num_prerequisites; j++)
        {
            int prerequisite_index = (int)json_array_get_number(prerequisites_array, j);
            upgrade_manager->upgrade_nodes[i].prerequisites[j] = &upgrade_manager->upgrade_nodes[prerequisite_index];
        }

        // assign next nodes
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
    upgrade_manager->current_node = &upgrade_manager->upgrade_nodes[0];                                                                                                  // set to root
    upgrade_manager->draw_offset = (Vector2){settings.game.target_size.x * 0.5f, 59};                                                                                    // center of target area
    upgrade_manager->camera_offset = (Vector2){0, 0};                                                                                                                    // pan camera based on this offset
    upgrade_manager->subtexture_hightlight_perk = resource_manager.get_subtexture(resource_manager.node_perk_mapper->node_perk_type_to_subtexture_id(3));                // set highlight subtexture
    upgrade_manager->subtexture_hightlight_attribute = resource_manager.get_subtexture(resource_manager.node_attribute_mapper->node_attribute_type_to_subtexture_id(3)); // set highlight subtexture
    upgrade_manager->input_manager = get_input_manager();
    upgrade_manager->font = resource_manager.get_pixel7_font();
    upgrade_manager->upgrade_display = &resource_manager.get_texture("upgrade-display")->texture;

    free(root_value);

    upgrade_manager->update = update;
    upgrade_manager->render = render;
    upgrade_manager->cleanup = cleanup;
    upgrade_manager->print_nodes = print_nodes;

    update_node_positions(upgrade_manager, (Vector2){0, 0}, 30); // assign positions to all the upgrade nodes
    update_node_states(upgrade_manager);                         // set states and textures based on prerequests and if purchased
    return upgrade_manager;
}
