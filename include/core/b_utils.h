#ifndef B_UTILS_H
#define B_UTILS_H

#include "raylib.h"
#include "box2d/box2d.h"
#include "resource.h"

const char *create_uuid(void);
void log_error(const char *message);
float flip_y(const float y);
float flip_y_center(const float y, const float size_y);
float y_center(const float y, const float size_y);
float x_center(const float x, const float size_x);
Vector2 vector2_flip_y(const Vector2 vec);
Vector2 vector2_flip_y_center(const Vector2 vec, const Vector2 size);
Vector2 vector2_center(const Vector2 vec, const Vector2 size);
Vector2 b2vec2_to_vector2(const b2Vec2 vec);
b2Vec2 vector2_to_b2vec2(const Vector2 vec);
Vector2 ints_to_vector2(const int x, const int y);
b2Vec2 ints_to_b2vec(const int x, const int y);
void render_subtexture(const Subtexture *subtexture, const b2Vec2 position);
void render_texture_scale(const Texture2D *texture, const b2Vec2 position, const float scale);
void render_texture_scale_color(const Texture2D *texture, const b2Vec2 position, const float scale, const Color color);

#endif