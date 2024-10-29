#include <stdio.h>
#include <time.h>
#include "uuid4.h"
#include "b_utils.h"
#include "settings.h"
#include "game.h"

const char *create_uuid(void)
{
    static char buf[UUID4_LEN];
    uuid4_init();
    uuid4_generate(buf);
    return buf;
}

void log_error(const char *message)
{
    FILE *logFile = fopen("error_log.txt", "a");
    if (logFile)
    {
        const time_t now = time(NULL);
        fprintf(logFile, "[%s] Error: %s\n", ctime(&now), message);
        fclose(logFile);
    }
    exit_game();
    TraceLog(LOG_ERROR, message);
}

float flip_y(const float y)
{
    return settings.game.target_size.y - y;
}

float y_center(const float y, const float size_y)
{
    return y - (size_y * 0.5f);
}

float flip_y_center(const float y, const float size_y)
{
    return y_center(flip_y(y), size_y);
}

float x_center(const float x, const float size_x)
{
    return x - (size_x * 0.5f);
}

Vector2 vector2_flip_y(const Vector2 vec)
{
    return (Vector2){vec.x, flip_y(vec.y)};
}

Vector2 vector2_flip_y_center(const Vector2 vec, const Vector2 size)
{
    return (Vector2){x_center(vec.x, size.x), flip_y_center(vec.y, size.y)};
}

Vector2 vector2_center(const Vector2 vec, const Vector2 size)
{
    return (Vector2){x_center(vec.x, size.x), y_center(vec.y, size.x)};
}

Vector2 b2vec2_to_vector2(const b2Vec2 vec)
{
    return (Vector2){vec.x, vec.y};
}

b2Vec2 vector2_to_b2vec2(const Vector2 vec)
{
    return (b2Vec2){vec.x, vec.y};
}

Vector2 ints_to_vector2(const int x, const int y)
{
    return (Vector2){(float)x, (float)y};
}

b2Vec2 ints_to_b2vec(const int x, const int y)
{
    return (b2Vec2){(float)x, (float)y};
}

void render_subtexture(const Subtexture *subtexture, const b2Vec2 position)
{
    const Vector2 size = (Vector2){subtexture->src.width, subtexture->src.height};
    DrawTextureRec(subtexture->texture_resource->texture,
                   subtexture->src,
                   vector2_flip_y_center(b2vec2_to_vector2(position), size),
                   WHITE);
}

// render_texture_scale // texture, size, scale
void render_texture_scale(const Texture2D *texture, const b2Vec2 position, const float scale)
{
    const Vector2 size = (Vector2){(float)texture->width * scale, (float)texture->height * scale};
    DrawTextureEx(*texture,
                  vector2_flip_y_center(b2vec2_to_vector2(position), size),
                  0.0f,
                  scale,
                  WHITE);
}

void render_texture_scale_color(const Texture2D *texture, const b2Vec2 position, const float scale, const Color color)
{
    const Vector2 size = (Vector2){(float)texture->width * scale, (float)texture->height * scale};
    DrawTextureEx(*texture,
                  vector2_flip_y_center(b2vec2_to_vector2(position), size),
                  0.0f,
                  scale,
                  color);
}