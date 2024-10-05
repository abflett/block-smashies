#include "raylib.h"
#include "title_scene.h"
#include "scene_manager.h"

static Scene title_scene;

static void scene_init(int arg_count, va_list args)
{
}

static void scene_update(float delta_time)
{
    // update information about the controller to be displayed.

    // if (GetKeyPressed() != 0 || IsGestureDetected(GESTURE_TAP))
    if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
    {
        scene_manager.change(scene_manager.scenes.main_menu, 0);
    }
}

static void scene_render(void)
{
    DrawText("Title Screen - (Animation for the Title Screen)", 5, 5, 8, WHITE);
    DrawText("Press ENTER to Main Menu", 5, 16, 8, WHITE);

    // display information about controllers, name of conroller and buttons pressed
    // Display connected controllers
    int connectedControllers = 0; // To count connected controllers
    // Display connected controllers
    for (int i = 0; i < 4; i++)
    {
        if (IsGamepadAvailable(i))
        {
            connectedControllers++; // Increment for each available controller

            const char *controllerName = GetGamepadName(i); // Get controller name
            int buttonPressed = GetGamepadButtonPressed();  // Get last button pressed
            int axisCount = GetGamepadAxisCount(i);         // Get axis count

            // Display the information in the desired format
            DrawText(TextFormat("%d Controller(s) connected", connectedControllers), 5, 40, 8, WHITE);
            DrawText(TextFormat("%s: Pressed: %d, %d axis", controllerName, buttonPressed, axisCount), 5, 60 + (i * 20), 8, WHITE);

            // Loop through axes and display their values
            for (int j = 0; j < axisCount; j++)
            {
                float axisValue = GetGamepadAxisMovement(i, j);
                DrawText(TextFormat("Axis%d: %.2f", j + 1, axisValue), 5, 80 + (i * 20) + (j * 9), 8, WHITE);
            }
        }
    }
}

static void scene_cleanup(void)
{
}

Scene *create_title_scene()
{
    title_scene.init = scene_init;
    title_scene.update = scene_update;
    title_scene.render = scene_render;
    title_scene.cleanup = scene_cleanup;

    return &title_scene;
};