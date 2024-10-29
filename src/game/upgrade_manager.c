#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "upgrade_manager.h"
#include "parson.h"
#include "settings.h"
#include "resource_manager.h"
#include "raymath.h"

static void update_node_states(const UpgradeManager *upgrade_manager)
{
    // Update purchased upgrades based on game_data
    for (int i = 0; i < upgrade_manager->game_data->num_purchased_upgrades; i++)
    {
        UpgradeNode *upgrade_node = &upgrade_manager->upgrade_nodes[upgrade_manager->game_data->purchased_upgrades[i]];
        upgrade_node->node_state = NODE_STATE_PURCHASED;
        const UpgradeType upgrade_type = upgrade_node->node_type->type;

        upgrade_node->background_subtexture = upgrade_type == UPGRADE_TYPE_NODE_PERK ? resource_manager.get_subtexture(resource_manager.node_perk_mapper->node_perk_type_to_subtexture_id((int)NODE_STATE_PURCHASED)) : resource_manager.get_subtexture(resource_manager.node_attribute_mapper->node_attribute_type_to_subtexture_id((int)NODE_STATE_PURCHASED));
    }

    // Update remaining nodes based on prerequisites
    for (int i = 0; i < upgrade_manager->node_count; i++)
    {
        UpgradeNode *node = &upgrade_manager->upgrade_nodes[i];

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

        const UpgradeType upgrade_type = node->node_type->type;
        node->background_subtexture = upgrade_type == UPGRADE_TYPE_NODE_PERK ? resource_manager.get_subtexture(resource_manager.node_perk_mapper->node_perk_type_to_subtexture_id((int)node->node_state)) : resource_manager.get_subtexture(resource_manager.node_attribute_mapper->node_attribute_type_to_subtexture_id((int)node->node_state));
    }
}

static void update_node_positions(const UpgradeManager *upgrade_manager)
{
    // Array to store node positions, indexed by layer, group, and node ID
    const float radius_increment = 30.0f;
    const Vector2 center = {0.0f, 0.0f};
    int layer_groups[24][24][24];
    memset(layer_groups, -1, sizeof(layer_groups)); // Initialize with -1

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
        const UpgradeNode *node = &upgrade_manager->upgrade_nodes[i];
        const int current_node_id = node->id;

        // Determine the layer based on prerequisites (distance from the root)
        layer = 0;
        const UpgradeNode *checked_node = node;

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
            const UpgradeNode *first_node_in_group = &upgrade_manager->upgrade_nodes[layer_groups[layer][g][0]];
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

        // Place the node in the next available position within the group
        node_id = 0;
        while (layer_groups[layer][group][node_id] != -1)
        {
            node_id++; // Find the next available slot in the group
        }
        layer_groups[layer][group][node_id] = current_node_id;
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
                const int current_node_id = layer_groups[layer_count][group_count][node];
                UpgradeNode *current_node = &upgrade_manager->upgrade_nodes[current_node_id];
                const float radius = radius_increment * (float)layer_count;

                if (layer_count <= 1)
                {
                    const float angle = 2 * PI / (float)nodes_count * (float)node;
                    current_node->position.x = center.x + radius * cosf(angle);
                    current_node->position.y = center.y + radius * sinf(angle);
                }
                else
                {
                    if (current_node->num_prerequisites > 1)
                    {
                        // Put node between the two prerequisites
                        const UpgradeNode *prerequisite1 = current_node->prerequisites[0];
                        const UpgradeNode *prerequisite2 = current_node->prerequisites[1];
                        const float angle_to_prerequisite1 = atan2f(prerequisite1->position.y - center.y, prerequisite1->position.x - center.x);
                        const float angle_to_prerequisite2 = atan2f(prerequisite2->position.y - center.y, prerequisite2->position.x - center.x);

                        // Calculate the average angle between the two prerequisites
                        float final_angle = (angle_to_prerequisite1 + angle_to_prerequisite2) / 2.0f;

                        // Ensure the angle is between 0 and 2*PI
                        if (final_angle < 0)
                        {
                            final_angle += 2 * PI;
                        }
                        else if (final_angle > 2 * PI)
                        {
                            final_angle -= 2 * PI;
                        }

                        // Set the node's position based on the final angle around the prerequisites
                        current_node->position.x = center.x + radius * cosf(final_angle);
                        current_node->position.y = center.y + radius * sinf(final_angle);
                    }
                    else
                    {
                        const UpgradeNode *prerequisite = current_node->prerequisites[0];
                        const float angle_to_prerequisite = atan2f(prerequisite->position.y - center.y, prerequisite->position.x - center.x);

                        // Calculate the angle offset based on the desired arc length and current radius
                        const float angle_offset = radius_increment / radius;

                        // Calculate the total spread based on the number of nodes
                        const float total_spread = angle_offset * (float)(nodes_count - 1);

                        // Calculate the starting angle offset to center the spread
                        const float start_offset = -total_spread * 0.5f;

                        // Calculate the final angle for this node
                        const float node_angle_offset = start_offset + angle_offset * (float)node;
                        const float final_angle = angle_to_prerequisite + node_angle_offset;

                        // Set the node's position based on the final angle around the prerequisite
                        current_node->position.x = center.x + radius * cosf(final_angle);
                        current_node->position.y = center.y + radius * sinf(final_angle);
                    }
                }
            }
        }
    }
}

static Color get_state_color(const UpgradeNode *node)
{
    const Color colors[3] =
        {
            settings.colors.blue_03, // NODE_STATE_LOCKED
            settings.colors.blue_05, // NODE_STATE_UNLOCKED
            settings.colors.blue_04  // NODE_STATE_PURCHASED
        };
    return colors[node->node_state];
}

static void update(UpgradeManager *upgrade_manager, const float delta_time)
{
    if (!upgrade_manager)
    {
        return;
    }

    upgrade_manager->exit_dialog->update(upgrade_manager->exit_dialog, delta_time);

    Vector2 temp_position = upgrade_manager->current_node->position;

    // Update highlight alpha
    upgrade_manager->highlight_alpha += (float)(upgrade_manager->highlight_alpha_increasing ? 1 : -1) * delta_time * 200.0f;

    // Clamp alpha between 0 and 255, and flip direction if bounds are reached
    if (upgrade_manager->highlight_alpha <= 100 || upgrade_manager->highlight_alpha >= 255)
    {
        upgrade_manager->highlight_alpha = fmaxf(100, fminf(255, upgrade_manager->highlight_alpha));
        upgrade_manager->highlight_alpha_increasing = !upgrade_manager->highlight_alpha_increasing;
    }

    bool moved = false;
    for (int player = 0; player < upgrade_manager->game_data->player_count; player++)
    {
        const InputManager *input_manager = upgrade_manager->input_manager;
        const InputMapping *input = input_manager->get_player_input(player);
        const int mapping = input_manager->player[player];

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
        // set and move to the closest node
        UpgradeNode *closest_node = upgrade_manager->current_node;
        float closest_distance = 99999999.99f;
        for (int i = 0; i < upgrade_manager->node_count; i++)
        {
            UpgradeNode *working_node = &upgrade_manager->upgrade_nodes[i];
            const float working_distance = Vector2Distance(temp_position, upgrade_manager->upgrade_nodes[i].position);

            if (working_distance < closest_distance && upgrade_manager->current_node != working_node)
            {
                closest_distance = working_distance;
                closest_node = working_node;
            }
        }
        upgrade_manager->current_node = closest_node;

        // move camera based on new current node
        const Vector2 center_position = upgrade_manager->upgrade_nodes[0].position;
        upgrade_manager->camera_offset = (Vector2){upgrade_manager->current_node->position.x - center_position.x, upgrade_manager->current_node->position.y - center_position.y};
    }
}

static void display_details(const UpgradeManager *upgrade_manager)
{
    const UpgradeNode *node = upgrade_manager->current_node;

    const char *cost_text = "";
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
    }

    // bottom left
    const char *node_type_text = TextFormat("%s - (%s)", node->node_type->name, node->node_type->type == UPGRADE_TYPE_NODE_PERK ? "Perk" : "Attribute");
    DrawTextEx(*upgrade_manager->font, node_type_text, (Vector2){44, 136}, 7, 0.0f, settings.colors.blue_05);

    DrawTextEx(*upgrade_manager->font, node->node_type->description, (Vector2){44, 149}, 7, 0.0f, settings.colors.blue_04);

    // bottom Right
    const Vector2 cost_text_size = MeasureTextEx(*upgrade_manager->font, cost_text, 7, 0.0f);
    DrawTextEx(*upgrade_manager->font, cost_text, (Vector2){277 - cost_text_size.x, 136}, 7, 0.0f, settings.colors.blue_04);

    const char *available_currency_text = TextFormat("Nanites: %d", upgrade_manager->game_data->currency);
    const Vector2 available_currency_text_size = MeasureTextEx(*upgrade_manager->font, available_currency_text, 7, 0.0f);
    DrawTextEx(*upgrade_manager->font, available_currency_text, (Vector2){277 - available_currency_text_size.x, 149}, 7, 0.0f, settings.colors.blue_04);

    const char *node_id_text = TextFormat("Id: %d", node->id);
    const Vector2 id_text_size = MeasureTextEx(*upgrade_manager->font, node_id_text, 7, 0.0f);
    DrawTextEx(*upgrade_manager->font, node_id_text, (Vector2){277 - id_text_size.x, 161}, 7, 0.0f, settings.colors.blue_04);
}

static void render(UpgradeManager *upgrade_manager)
{
    if (!upgrade_manager)
    {
        return;
    }

    ClearBackground(settings.colors.blue_01);
    // Set the draw offset
    const Vector2 offset = Vector2Subtract(upgrade_manager->draw_offset, upgrade_manager->camera_offset);

    // Draw connections between nodes and their prerequisites, this first so lines are behind nodes
    for (int i = 0; i < upgrade_manager->node_count; i++)
    {
        const UpgradeNode *node = &upgrade_manager->upgrade_nodes[i];
        for (int j = 0; j < node->num_prerequisites; j++)
        {
            const UpgradeNode *prerequisite = node->prerequisites[j];

            DrawLineEx((Vector2){(prerequisite->position.x + offset.x), (prerequisite->position.y + offset.y)},
                       (Vector2){(node->position.x + offset.x), (node->position.y + offset.y)}, 2.0f, settings.colors.blue_02);
        }
    }

    // Draw nodes
    for (int i = 0; i < upgrade_manager->node_count; i++)
    {
        const UpgradeNode *node = &upgrade_manager->upgrade_nodes[i];
        // draw node highlight
        if (node == upgrade_manager->current_node)
        {
            const Subtexture *subtexture_highlight = node->node_type->highlight_subtexture;
            const Vector2 highlight_size = (Vector2){subtexture_highlight->src.width, subtexture_highlight->src.height};
            DrawTexturePro(subtexture_highlight->texture_resource->texture, subtexture_highlight->src, (Rectangle){node->position.x + offset.x - highlight_size.x * 0.5f + 1, node->position.y + offset.y - highlight_size.y * 0.5f, highlight_size.x, highlight_size.y}, (Vector2){1, 0}, 0, (Color){255, 255, 255, (unsigned char)upgrade_manager->highlight_alpha});
        }

        // draw node body
        const Vector2 size = (Vector2){node->background_subtexture->src.width, node->background_subtexture->src.height};
        DrawTexturePro(node->background_subtexture->texture_resource->texture, node->background_subtexture->src, (Rectangle){node->position.x + offset.x - size.x * 0.5f + 1, node->position.y + offset.y - size.y * 0.5f, size.x, size.y}, (Vector2){1, 0}, 0, WHITE);

        // draw node icon
        const Color icon_color = get_state_color(node);
        const Vector2 icon_size = (Vector2){node->node_type->icon_subtexture->src.width, node->node_type->icon_subtexture->src.height};

        DrawTexturePro(node->node_type->icon_subtexture->texture_resource->texture, node->node_type->icon_subtexture->src, (Rectangle){node->position.x + offset.x - icon_size.x * 0.5f + 1, node->position.y + offset.y - icon_size.y * 0.5f, icon_size.x, icon_size.y}, (Vector2){1, 0}, 0, icon_color);
    }

    // Draw the upgrade display
    DrawTexture(*upgrade_manager->upgrade_display, 0, 0, WHITE);

    display_details(upgrade_manager);

    upgrade_manager->exit_dialog->render(upgrade_manager->exit_dialog);
}

static void cleanup(UpgradeManager *upgrade_manager)
{
    if (upgrade_manager)
    {
        for (int i = 0; i < upgrade_manager->node_count; i++)
        {
            free(upgrade_manager->upgrade_nodes[i].prerequisites);
        }

        for (int i = 0; i < 18; i++)
        {
            free(upgrade_manager->upgrade_node_types[i].name);
            free(upgrade_manager->upgrade_node_types[i].description);
        }

        free(upgrade_manager->upgrade_nodes);
        free(upgrade_manager);
    }
}

UpgradeManager *create_upgrade_manager(GameData *game_data)
{
    UpgradeManager *upgrade_manager = malloc(sizeof(UpgradeManager));
    upgrade_manager->game_data = game_data;
    upgrade_manager->subtexture_highlight_types[0] = resource_manager.get_subtexture(resource_manager.node_perk_mapper->node_perk_type_to_subtexture_id(3));
    upgrade_manager->subtexture_highlight_types[1] = resource_manager.get_subtexture(resource_manager.node_attribute_mapper->node_attribute_type_to_subtexture_id(3));

    JSON_Value *root_value = json_parse_file(settings.file_locations.upgrade_file);
    const JSON_Object *root_object = json_value_get_object(root_value);

    const JSON_Array *node_types_array = json_object_get_array(root_object, "node-types");
    for (int i = 0; i < json_array_get_count(node_types_array); i++)
    {
        const JSON_Object *node_type_object = json_array_get_object(node_types_array, i);
        upgrade_manager->upgrade_node_types[i].type_id = (UpgradeTypeId)json_object_get_number(node_type_object, "type_id");
        upgrade_manager->upgrade_node_types[i].type = (UpgradeType)json_object_get_number(node_type_object, "type");
        upgrade_manager->upgrade_node_types[i].name = strdup(json_object_get_string(node_type_object, "name"));
        upgrade_manager->upgrade_node_types[i].description = strdup(json_object_get_string(node_type_object, "description"));
        upgrade_manager->upgrade_node_types[i].highlight_subtexture = upgrade_manager->upgrade_node_types[i].type == UPGRADE_TYPE_NODE_PERK ? upgrade_manager->subtexture_highlight_types[0] : upgrade_manager->subtexture_highlight_types[1];
        upgrade_manager->upgrade_node_types[i].icon_subtexture = resource_manager.get_subtexture(resource_manager.upgrade_icon_mapper->upgrade_type_to_subtexture_id(i));
    }

    const JSON_Array *upgrades_array = json_object_get_array(root_object, "upgrades");
    upgrade_manager->node_count = (int)json_array_get_count(upgrades_array);
    upgrade_manager->upgrade_nodes = malloc(sizeof(UpgradeNode) * upgrade_manager->node_count);
    for (int i = 0; i < upgrade_manager->node_count; i++)
    {
        const JSON_Object *upgrade_object = json_array_get_object(upgrades_array, i);

        // Populate each node directly
        upgrade_manager->upgrade_nodes[i].id = (int)json_object_get_number(upgrade_object, "id");
        upgrade_manager->upgrade_nodes[i].node_type = &upgrade_manager->upgrade_node_types[(int)json_object_get_number(upgrade_object, "type_id")];
        upgrade_manager->upgrade_nodes[i].cost = (int)json_object_get_number(upgrade_object, "cost");
        upgrade_manager->upgrade_nodes[i].value = (float)json_object_get_number(upgrade_object, "value");
        upgrade_manager->upgrade_nodes[i].node_state = NODE_STATE_LOCKED;
        upgrade_manager->upgrade_nodes[i].position = (Vector2){0, 0}; // Initialize position
        const bool is_perk = upgrade_manager->upgrade_nodes[i].node_type->type == UPGRADE_TYPE_NODE_PERK;
        upgrade_manager->upgrade_nodes[i].background_subtexture = is_perk ? resource_manager.get_subtexture(resource_manager.node_perk_mapper->node_perk_type_to_subtexture_id((int)upgrade_manager->upgrade_nodes[i].node_state)) : resource_manager.get_subtexture(resource_manager.node_attribute_mapper->node_attribute_type_to_subtexture_id((int)upgrade_manager->upgrade_nodes[i].node_state));

        // assign prerequisites
        const JSON_Array *prerequisites_array = json_object_get_array(upgrade_object, "prerequisites");
        upgrade_manager->upgrade_nodes[i].num_prerequisites = (int)json_array_get_count(prerequisites_array);
        upgrade_manager->upgrade_nodes[i].prerequisites = malloc(sizeof(UpgradeNode *) * upgrade_manager->upgrade_nodes[i].num_prerequisites);
        for (int j = 0; j < upgrade_manager->upgrade_nodes[i].num_prerequisites; j++)
        {
            const int prerequisite_index = (int)json_array_get_number(prerequisites_array, j);
            upgrade_manager->upgrade_nodes[i].prerequisites[j] = &upgrade_manager->upgrade_nodes[prerequisite_index];
        }
    }

    free(root_value);

    // Set the current node to the first node (optional)
    upgrade_manager->current_node = &upgrade_manager->upgrade_nodes[0];               // set to root
    upgrade_manager->draw_offset = (Vector2){settings.game.target_size.x * 0.5f, 59}; // center of target area
    upgrade_manager->camera_offset = (Vector2){0, 0};                                 // pan camera based on this offset
    upgrade_manager->input_manager = get_input_manager();
    upgrade_manager->font = resource_manager.get_pixel7_font();
    upgrade_manager->upgrade_display = &resource_manager.get_texture("upgrade-display")->texture;
    upgrade_manager->highlight_alpha = 0.0f;
    upgrade_manager->highlight_alpha_increasing = true;

    upgrade_manager->update = update;
    upgrade_manager->render = render;
    upgrade_manager->cleanup = cleanup;

    upgrade_manager->active = true;

    upgrade_manager->exit_dialog = create_game_dialog("Are sure you are sure\nyou want to exit?", (Vector2){50, 50});

    // Todo: add a function to calculate the value of the upgrade based on the type and id
    // Todo: add a function to display the value of the upgrade

    update_node_positions(upgrade_manager); // assign positions to all the upgrade nodes
    update_node_states(upgrade_manager);                         // set states and textures based on prerequisites and if purchased
    return upgrade_manager;
}
