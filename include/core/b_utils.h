#ifndef B_UTILS_H
#define B_UTILS_H

#include "raylib.h"
#include "box2d/box2d.h"
#include "resource.h"

const char *create_uuid(void);
void log_error(const char *message);
float flip_y(float y);
float flip_y_center(float y, float size_y);
float y_center(float y, float size_y);
float x_center(float x, float size_x);
Vector2 vector2_flip_y(Vector2 vec);
Vector2 vector2_flip_y_center(Vector2 vec, Vector2 size);
Vector2 vector2_center(Vector2 vec, Vector2 size);
Vector2 b2vec2_to_vector2(b2Vec2 vec);
b2Vec2 vector2_to_b2vec2(Vector2 vec);
Vector2 ints_to_vector2(int x, int y);
b2Vec2 ints_to_b2vec(int x, int y);
void render_subtexture(Subtexture *subtexture, b2Vec2 position);
void render_texture_scale(Texture2D *texture, b2Vec2 position, float scale);
void render_texture_scale_color(Texture2D *texture, b2Vec2 position, float scale, Color color);

#endif