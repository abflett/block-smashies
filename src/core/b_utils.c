#include <stdio.h>
#include <time.h>
#include "b_utils.h"
#include "settings.h"
#include "game.h"

void log_error(const char *message)
{
    FILE *logFile = fopen("error_log.txt", "a");
    if (logFile)
    {
        time_t now = time(NULL);
        fprintf(logFile, "[%s] Error: %s\n", ctime(&now), message);
        fclose(logFile);
    }
    exit_game();
    TraceLog(LOG_ERROR, message);
}

float flip_y(float y)
{
    return settings.game.target_size.y - y;
}

float y_center(float y, float size_y)
{
    return y - (size_y * 0.5f);
}

float flip_y_center(float y, float size_y)
{
    return y_center(flip_y(y), size_y);
}

float x_center(float x, float size_x)
{
    return x - (size_x * 0.5f);
}

Vector2 vector2_flip_y(Vector2 vec)
{
    return (Vector2){vec.x, flip_y(vec.y)};
}

Vector2 vector2_flip_y_center(Vector2 vec, Vector2 size)
{
    return (Vector2){x_center(vec.x, size.x), flip_y_center(vec.y, size.y)};
}

Vector2 vector2_center(Vector2 vec, Vector2 size)
{
    return (Vector2){x_center(vec.x, size.x), y_center(vec.y, size.x)};
}

Vector2 b2vec2_to_vector2(b2Vec2 vec)
{
    return (Vector2){vec.x, vec.y};
}

b2Vec2 vector2_to_b2vec2(Vector2 vec)
{
    return (b2Vec2){vec.x, vec.y};
}

Vector2 ints_to_vector2(int x, int y)
{
    return (Vector2){(float)x, (float)y};
}

b2Vec2 ints_to_b2vec(int x, int y)
{
    return (b2Vec2){(float)x, (float)y};
}

void render_subtexture(Subtexture *subtexture, b2Vec2 position)
{
    Vector2 size = (Vector2){subtexture->src.width, subtexture->src.height};
    DrawTextureRec(subtexture->texture_resource->texture,
                   subtexture->src,
                   vector2_flip_y_center(b2vec2_to_vector2(position), size),
                   WHITE);
}

// render_texture_scale // texture, size, scale
void render_texture_scale(Texture2D *texture, b2Vec2 position, float scale)
{
    Vector2 size = (Vector2){texture->width * scale, texture->height * scale};
    DrawTextureEx(*texture,
                  vector2_flip_y_center(b2vec2_to_vector2(position), size),
                  0.0f,
                  scale,
                  WHITE);
}

void render_texture_scale_color(Texture2D *texture, b2Vec2 position, float scale, Color color)
{
    Vector2 size = (Vector2){texture->width * scale, texture->height * scale};
    DrawTextureEx(*texture,
                  vector2_flip_y_center(b2vec2_to_vector2(position), size),
                  0.0f,
                  scale,
                  color);
}