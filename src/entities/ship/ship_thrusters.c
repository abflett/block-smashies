#include "raylib.h"
#include "ship_thrusters.h"
#include "resource_manager.h"
#include "b_utils.h"

const float segment_body_width = 7.0f;             // Width of each ship segment
const float thruster_vertical_right_offset = 4.0f; // Offset from ship body to thrusters vertically
const float thruster_vertical_left_offset = 3.0f;  // Offset from ship body to thrusters vertically
const float thruster_horizontal_offset = 1.0f;     // Offset for left and right thrusters
const float bottom_thruster_offset_y = 9.0f;       // Y offset for bottom thrusters
const float bottom_thruster_x_padding = 3.0f;      // Padding for bottom thruster X positioning

static void render_ship_thrusters(const ShipThrusters *ship_thrusters)
{
    // Calculate widths
    const float full_body_width = segment_body_width * (float)(*ship_thrusters->segments);
    const float half_body_width = full_body_width * 0.5f;

    // Left and right thruster positions
    const Vector2 left_position = (Vector2){ship_thrusters->position->x - half_body_width - thruster_horizontal_offset,
                                      flip_y(ship_thrusters->position->y - thruster_vertical_left_offset)};
    const Vector2 right_position = (Vector2){ship_thrusters->position->x + half_body_width + thruster_horizontal_offset,
                                       flip_y(ship_thrusters->position->y - thruster_vertical_right_offset)};
    const float bottom_y_position = flip_y(ship_thrusters->position->y - bottom_thruster_offset_y);

    // Update and render left thruster
    ship_thrusters->left_thruster->update(ship_thrusters->left_thruster, GetFrameTime(), 90.0f);
    ship_thrusters->left_thruster->render(ship_thrusters->left_thruster, left_position);

    // Update and render right thruster
    ship_thrusters->right_thruster->update(ship_thrusters->right_thruster, GetFrameTime(), -90.0f);
    ship_thrusters->right_thruster->render(ship_thrusters->right_thruster, right_position);

    // Update and render bottom thrusters for each segment
    ship_thrusters->bottom_thruster->update(ship_thrusters->bottom_thruster, GetFrameTime(), 0.0f);
    for (int i = 0; i < *ship_thrusters->segments; i++)
    {
        // Calculate the position for each bottom thruster segment
        const Vector2 bottom_position = (Vector2){
            ship_thrusters->position->x - half_body_width + bottom_thruster_x_padding + ((float)i * segment_body_width),
            bottom_y_position};

        ship_thrusters->bottom_thruster->render(ship_thrusters->bottom_thruster, bottom_position);
    }
}

static void cleanup_ship_thrusters(ShipThrusters *ship_thrusters)
{
    ship_thrusters->left_thruster->cleanup(ship_thrusters->left_thruster);
    ship_thrusters->right_thruster->cleanup(ship_thrusters->right_thruster);
    ship_thrusters->bottom_thruster->cleanup(ship_thrusters->bottom_thruster);
    free(ship_thrusters);
}

ShipThrusters *create_ship_thrusters(int *segments, b2Vec2 *position)
{
    ShipThrusters *ship_thrusters = malloc(sizeof(ShipThrusters));

    ship_thrusters->segments = segments;
    ship_thrusters->position = position;

    ship_thrusters->left_level = 0;
    ship_thrusters->right_level = 0;
    ship_thrusters->bottom_level = 0;

    const char *animation_left_id = resource_manager.thruster_level_mapper->thruster_level_to_animation_id(4);
    const char *animation_right_id = resource_manager.thruster_level_mapper->thruster_level_to_animation_id(4);
    const char *animation_bottom_id = resource_manager.thruster_level_mapper->thruster_level_to_animation_id(4);

    ship_thrusters->left_thruster = create_animation_manager(animation_left_id, ANIMATION_PING_PONG, 0.03f);
    ship_thrusters->left_thruster->is_playing = true;

    ship_thrusters->right_thruster = create_animation_manager(animation_right_id, ANIMATION_PING_PONG, 0.03f);
    ship_thrusters->right_thruster->is_playing = true;

    ship_thrusters->bottom_thruster = create_animation_manager(animation_bottom_id, ANIMATION_PING_PONG, 0.03f);
    ship_thrusters->bottom_thruster->is_playing = true;

    ship_thrusters->render = render_ship_thrusters;
    ship_thrusters->cleanup = cleanup_ship_thrusters;

    return ship_thrusters;
}
