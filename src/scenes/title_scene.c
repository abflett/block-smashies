#include "raylib.h"
#include "title_scene.h"
#include "scene_manager.h"
#include "gamepad_test.h"
#include "gamepad.h"

static Scene title_scene;
static GamepadTest *gamepad_test;

static void scene_init(int arg_count, va_list args)
{
    gamepad_test = create_gamepad_test();
}

static void scene_update(float delta_time)
{
    gamepad_test->update(delta_time);

    // if (GetKeyPressed() != 0 || IsGestureDetected(GESTURE_TAP))
    if (IsKeyPressed(KEY_ENTER) /* || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)*/)
    {
        scene_manager.change(scene_manager.scenes.main_menu, 0);
    }
}

static void scene_render(void)
{
    gamepad_test->render();
}

static void scene_cleanup(void)
{
    gamepad_test->cleanup();
}

Scene *create_title_scene()
{
    title_scene.init = scene_init;
    title_scene.update = scene_update;
    title_scene.render = scene_render;
    title_scene.cleanup = scene_cleanup;

    return &title_scene;
};