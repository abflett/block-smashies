
#include "parson.h"
#include "level_manager.h"
#include "settings.h"
#include "b_utils.h"
#include "game_context.h"

static LevelManager manager;

static void load_level(void)
{
    TraceLog(LOG_INFO, "Loading level");

    JSON_Value *root_value = json_parse_file(settings.file_locations.levels_file);
    if (root_value == NULL)
    {
        log_error("Error reading JSON file");
        return;
    }

    JSON_Array *level_array = json_value_get_array(root_value);
    size_t level_count = json_array_get_count(level_array);

    // Iterate over the levels to find the one that matches the current mission and operation
    for (size_t i = 0; i < level_count; i++)
    {
        JSON_Object *level_object = json_array_get_object(level_array, i);

        int operation = (int)json_object_get_number(level_object, "operation");
        int mission = (int)json_object_get_number(level_object, "mission");

        // If the current level matches the mission and operation from game_data
        if (operation == manager.context->game_data->operation && mission == manager.context->game_data->mission)
        {
            // Parse the time and grid
            manager.level_data.time = (float)json_object_get_number(level_object, "time");

            JSON_Array *grid_array = json_object_get_array(level_object, "grid");

            // Ensure grid size matches expected 12x12
            if (json_array_get_count(grid_array) != 12 * 12)
            {
                log_error("Grid size does not match 12x12.");
                return;
            }

            // Populate the level_data grid
            for (int j = 0; j < 12 * 12; j++)
            {
                manager.level_data.data[j] = (int)json_array_get_number(grid_array, j);
            }

            // Set mission and operation
            manager.level_data.mission = mission;
            manager.level_data.operation = operation;

            // Exit after the correct level is loaded
            break;
        }
    }

    json_value_free(root_value); // Free the root JSON value after processing
}

static void create_level(void)
{
    manager.context->game_status->enemy_count = 0;

    // Define row and column sizes, assuming 12x12 grid
    int brick_row = 12;
    int brick_column = 12;

    // Define spacing for rows and columns
    float row_spacing = 18.0f; // Horizontal spacing (width of the brick + any gap)
    float col_spacing = 8.0f;  // Vertical spacing (height of the brick + any gap)

    // Loop through the grid data in level_data
    for (int col = 0; col < brick_row; col++)
    {
        for (int row = 0; row < brick_column; row++)
        {
            // Access the correct index in the flat data array (12x12 grid stored as a flat array)
            int brick_type = manager.level_data.data[col * brick_column + row];

            // Only add bricks if the brick_type is not 0 (assuming 0 means empty space)
            if (brick_type != 0)
            {
                // Calculate the position of the brick based on its row/col index and spacing
                b2Vec2 position = {
                    row * row_spacing + settings.game.play_area.x + 10,                               // X position
                    settings.game.target_size.y - (col * col_spacing + settings.game.play_area.y + 5) // Y position (adjust for screen coords)
                };

                // Add the brick using the brick_type
                manager.context->entities->add_brick(position, brick_type - 1);

                manager.context->game_status->enemy_count++;
            }
        }
    }
}

LevelManager *create_level_manager(GameContext *context)
{

    manager.context = context;

    // function pointers
    manager.load_level = load_level;     // Assign the load_level function
    manager.create_level = create_level; // This should be a function you implement for level creation

    manager.load_level();

    return &manager;
}