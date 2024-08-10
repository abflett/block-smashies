#include "raylib.h"

typedef enum GameScreen
{
    LOGO = 0,
    TITLE,
    GAMEPLAY,
    ENDING
} GameScreen;

int main()
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    bool exitWindowRequested = false;
    bool exitWindow = false;
    int framesCounter = 0;

    GameScreen currentScreen = LOGO;

    InitWindow(screenWidth, screenHeight, "Block Smashies");
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    while (!exitWindow)
    {
        if (WindowShouldClose() || IsKeyPressed(KEY_ESCAPE))
            exitWindowRequested = true;

        if (exitWindowRequested)
        {
            if (IsKeyPressed(KEY_Y))
                exitWindow = true;
            else if (IsKeyPressed(KEY_N))
                exitWindowRequested = false;
        }

        switch (currentScreen)
        {
        case LOGO:
        {
            framesCounter++; // Count frames
            if (framesCounter > 120)
            {
                currentScreen = TITLE;
            }
        }
        break;
        case TITLE:
        {
            if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
            {
                currentScreen = GAMEPLAY;
            }
        }
        break;
        case GAMEPLAY:
        {
            if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
            {
                currentScreen = ENDING;
            }
        }
        break;
        case ENDING:
        {
            if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
            {
                currentScreen = TITLE;
            }
        }
        break;
        default:
            break;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        if (exitWindowRequested)
        {
            DrawRectangle(0, 100, screenWidth, 200, DARKGRAY);
            DrawText("Are you sure you want to exit program? [Y/N]", 40, 180, 30, WHITE);
        }
        else
        {
            switch (currentScreen)
            {
            case LOGO:
            {
                DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
                DrawText("WAIT for 2 SECONDS...", 290, 220, 20, GRAY);
            }
            break;
            case TITLE:
            {
                DrawRectangle(0, 0, screenWidth, screenHeight, GREEN);
                DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
                DrawText("PRESS ENTER or TAP to JUMP to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);
            }
            break;
            case GAMEPLAY:
            {
                DrawRectangle(0, 0, screenWidth, screenHeight, PURPLE);
                DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
                DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);
            }
            break;
            case ENDING:
            {
                DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
                DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
                DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 120, 220, 20, DARKBLUE);
            }
            break;
            default:
                break;
            }
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}