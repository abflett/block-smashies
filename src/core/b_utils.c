#include <stdio.h>
#include <time.h>
#include "raylib.h"
#include "box2d/box2d.h"
#include "b_utils.h"
#include "settings.h"

void log_error(const char *message)
{
    FILE *logFile = fopen("error_log.txt", "a");
    if (logFile)
    {
        time_t now = time(NULL);
        fprintf(logFile, "[%s] Error: %s\n", ctime(&now), message);
        fclose(logFile);
    }
}

float flip_y(float y)
{
    return settings.game.target_size.y - y;
}

float flip_y_center(float y, float size_y)
{
    return settings.game.target_size.y - (y + (size_y / 2));
}

float y_center(float y, float size_y)
{
    return y + (size_y / 2);
}

float x_center(float x, float size_x)
{
    return x + (size_x / 2);
}

Vector2 b2vec2_to_vector2(b2Vec2 vec)
{
    return (Vector2){vec.x, vec.y};
}

b2Vec2 vector2_to_b2vec2(Vector2 vec)
{
    return (b2Vec2){vec.x, vec.y};
}

Vector2 vector2_flip_y(Vector2 vec)
{
    return (Vector2){vec.x, flip_y(vec.y)};
}

Vector2 vector2_flip_y_center(Vector2 vec, Vector2 size)
{
    return (Vector2){x_center(vec.x, size.x), flip_y_center(vec.y, size.x)};
}

Vector2 vector2_center(Vector2 vec, Vector2 size)
{
    return (Vector2){x_center(vec.x, size.x), y_center(vec.y, size.x)};
}