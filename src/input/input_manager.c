#include "raylib.h"
#include "input_manager.h"
#include "settings.h"

static InputManager manager;

InputManager *create_input_manager(void)
{
    return &manager;
}