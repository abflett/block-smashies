#include "box2d/box2d.h"
#include "raylib.h"
#include "animation_handler.h"
#include "resource_manager.h"
#include "settings.h"

static void update_animation(AnimationHandler *animation_handler, float delta_time)
{
    if (animation_handler->is_playing)
    {
        animation_handler->elapsed_time += delta_time;

        if (animation_handler->elapsed_time >= animation_handler->frame_time)
        {
            animation_handler->elapsed_time -= animation_handler->frame_time;
            animation_handler->frame_index++;

            if (animation_handler->animation_type == ANIMATION_LOOP)
            {
                animation_handler->frame_index %= animation_handler->frame_count;
            }
            else if (animation_handler->animation_type == ANIMATION_ONCE)
            {
                if (animation_handler->frame_index >= animation_handler->frame_count)
                {
                    animation_handler->is_playing = false;
                }
            }
        }
    }
}

static void render_animation(AnimationHandler *animation_handler, b2Vec2 position)
{
    if (animation_handler->is_playing)
    {
        DrawTextureRec(animation_handler->animation->texture_resource->texture,
                       animation_handler->animation->frames[animation_handler->frame_index],
                       (Vector2){position.x, settings.game.target_size.y - position.y},
                       WHITE);
    }
}

static void cleanup_animation(AnimationHandler *animation_handler)
{
    free(animation_handler);
}

AnimationHandler *create_animation_manager(const char *id, AnimationType animation_type, float frame_time)
{
    AnimationHandler *animation_handler = (AnimationHandler *)malloc(sizeof(AnimationHandler));
    animation_handler->animation = resource_manager.get_animation(id);
    animation_handler->animation_type = animation_type;
    animation_handler->frame_index = 0;
    animation_handler->frame_count = animation_handler->animation->frame_count;
    animation_handler->is_playing = false;
    animation_handler->frame_time = frame_time;
    animation_handler->elapsed_time = 0.0f;

    animation_handler->update = update_animation;
    animation_handler->render = render_animation;
    animation_handler->cleanup = cleanup_animation;

    return animation_handler;
}
