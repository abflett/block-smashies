#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "uthash.h"
#include "raylib.h"
#include "parson.h"
#include "resource_manager.h"

static void add_texture(TextureResource *texture)
{
    TraceLog(LOG_INFO, "add_texture(TextureResource *texture) called");
    HASH_ADD_KEYPTR(hh, resource_manager.textures, texture->id, strlen(texture->id), texture);
}

static void add_subtexture(Subtexture *subtexture)
{
    TraceLog(LOG_INFO, "add_subtexture(Subtexture *subtexture) called");
    HASH_ADD_KEYPTR(hh, resource_manager.subtextures, subtexture->id, strlen(subtexture->id), subtexture);
}

static void add_animation(Animation *animation)
{
    TraceLog(LOG_INFO, "add_animation(Animation *animation) called");
    HASH_ADD_KEYPTR(hh, resource_manager.animations, animation->id, strlen(animation->id), animation);
}

void rm_load_resource_file(const char *file)
{
    TraceLog(LOG_INFO, "rm_load_resource_file(const char *file) called");

    // Parse the JSON file
    JSON_Value *root_value = json_parse_file(file);
    JSON_Object *root_object = json_value_get_object(root_value);
    JSON_Array *textures_array = json_object_get_array(root_object, "textures");

    // Iterate through textures
    for (size_t i = 0; i < json_array_get_count(textures_array); i++)
    {
        JSON_Object *texture_obj = json_array_get_object(textures_array, i);
        const char *id = json_object_get_string(texture_obj, "id");
        const char *file = json_object_get_string(texture_obj, "file");
        const char *type = json_object_get_string(texture_obj, "type");

        TraceLog(LOG_INFO, "Texture ID: %s", id);
        TraceLog(LOG_INFO, "File: %s", file);
        TraceLog(LOG_INFO, "Type: %s", type);

        if (strcmp(type, "single") == 0)
        {
            // Load single texture
            TextureResource *texture = malloc(sizeof(TextureResource));
            texture->id = strdup(id); // Copy string to avoid dangling pointer
            texture->texture = LoadTexture(file);
            add_texture(texture);
        }
        else if (strcmp(type, "atlas") == 0)
        {
            // Load texture atlas
            TextureResource *atlas_texture = malloc(sizeof(TextureResource));
            atlas_texture->id = strdup(id); // Copy string to avoid dangling pointer
            atlas_texture->texture = LoadTexture(file);
            add_texture(atlas_texture);

            // Load subtextures
            JSON_Array *subtextures_array = json_object_get_array(texture_obj, "subtextures");
            for (size_t j = 0; j < json_array_get_count(subtextures_array); j++)
            {
                JSON_Object *subtexture_obj = json_array_get_object(subtextures_array, j);
                const char *sub_id = json_object_get_string(subtexture_obj, "id");
                JSON_Object *rect_obj = json_object_get_object(subtexture_obj, "rect");

                Subtexture *subtexture = malloc(sizeof(Subtexture));
                subtexture->id = strdup(sub_id); // Copy string to avoid dangling pointer
                subtexture->rect.x = (float)json_object_get_number(rect_obj, "x");
                subtexture->rect.y = (float)json_object_get_number(rect_obj, "y");
                subtexture->rect.width = (float)json_object_get_number(rect_obj, "w");
                subtexture->rect.height = (float)json_object_get_number(rect_obj, "h");
                subtexture->texture_resource = atlas_texture;
                add_subtexture(subtexture);
            }

            // Load animations
            JSON_Array *animations_array = json_object_get_array(texture_obj, "animations");
            for (size_t j = 0; j < json_array_get_count(animations_array); j++)
            {
                JSON_Object *animation_obj = json_array_get_object(animations_array, j);
                const char *anim_id = json_object_get_string(animation_obj, "id");
                int frame_count = (int)json_object_get_number(animation_obj, "frame_count");
                int framerate = (int)json_object_get_number(animation_obj, "framerate");

                Animation *animation = malloc(sizeof(Animation));
                animation->id = strdup(anim_id); // Copy string to avoid dangling pointer
                animation->frame_count = frame_count;
                animation->framerate = framerate;
                animation->subtexture_frames = malloc(sizeof(Subtexture) * frame_count);

                JSON_Array *frames_array = json_object_get_array(animation_obj, "frames");
                for (int k = 0; k < frame_count; k++)
                {
                    JSON_Object *frame_obj = json_array_get_object(frames_array, k);
                    JSON_Object *rect_obj = json_object_get_object(frame_obj, "rect");

                    Subtexture *frame = malloc(sizeof(Subtexture));
                    frame->rect.x = (float)json_object_get_number(rect_obj, "x");
                    frame->rect.y = (float)json_object_get_number(rect_obj, "y");
                    frame->rect.width = (float)json_object_get_number(rect_obj, "w");
                    frame->rect.height = (float)json_object_get_number(rect_obj, "h");
                    frame->texture_resource = atlas_texture;

                    animation->subtexture_frames[k] = *frame;
                }
                add_animation(animation);
            }
        }
    }

    json_value_free(root_value);
}

TextureResource *rm_get_texture(const char *id)
{
    TraceLog(LOG_INFO, "Searching for texture with id: %s", id);
    TextureResource *texture = NULL;
    HASH_FIND_STR(resource_manager.textures, id, texture);

    if (texture)
    {
        TraceLog(LOG_INFO, "Texture found with id: %s", id);
        return texture;
    }
    else
    {
        TraceLog(LOG_WARNING, "Texture with id '%s' not found", id);
        return NULL;
    }
}

Subtexture *rm_get_subtexture(const char *id)
{
    Subtexture *subtexture = NULL;
    HASH_FIND_STR(resource_manager.subtextures, id, subtexture);
    return subtexture;
}

Animation *rm_get_animation(const char *id)
{
    Animation *animation = NULL;
    HASH_FIND_STR(resource_manager.animations, id, animation);
    return animation;
}

void rm_cleanup(void)
{
    TraceLog(LOG_INFO, "rm_cleanup(void) called");

    // Free all textures
    TextureResource *texture, *tmp_texture;
    HASH_ITER(hh, resource_manager.textures, texture, tmp_texture)
    {
        UnloadTexture(texture->texture);
        HASH_DEL(resource_manager.textures, texture);
        free((char *)texture->id); // Free duplicated string
        free(texture);
    }

    // Free all subtextures
    Subtexture *subtexture, *tmp_subtexture;
    HASH_ITER(hh, resource_manager.subtextures, subtexture, tmp_subtexture)
    {
        HASH_DEL(resource_manager.subtextures, subtexture);
        free((char *)subtexture->id); // Free duplicated string
        free(subtexture);
    }

    // Free all animations and their frames
    Animation *animation, *tmp_animation;
    HASH_ITER(hh, resource_manager.animations, animation, tmp_animation)
    {
        free(animation->subtexture_frames);
        HASH_DEL(resource_manager.animations, animation);
        free((char *)animation->id); // Free duplicated string
        free(animation);
    }

    resource_manager.textures = NULL;
    resource_manager.subtextures = NULL;
    resource_manager.animations = NULL;
}

ResourceManager resource_manager = {
    .load_resource_file = rm_load_resource_file,
    .get_texture = rm_get_texture,
    .get_subtexture = rm_get_subtexture,
    .get_animation = rm_get_animation,
    .cleanup = rm_cleanup,
    .textures = NULL,
    .subtextures = NULL,
    .animations = NULL};