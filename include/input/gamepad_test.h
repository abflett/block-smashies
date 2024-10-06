#ifndef GAMEPAD_TEST_H
#define GAMEPAD_TEST_H

typedef struct GamepadTest
{
    void (*update)(float delta_time);
    void (*render)(void);
    void (*cleanup)(void);
} GamepadTest;

GamepadTest *create_gamepad_test(void);

#endif