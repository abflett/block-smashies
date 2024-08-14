#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "parson.h"
#include "resource_manager.h"

// Dummy implementations of the helper functions
static void add_texture(TextureResource *texture)
{
    TraceLog(LOG_INFO, "add_texture(TextureResource *texture) called");

    resource_manager.texture_count++;
    resource_manager.textures = realloc(resource_manager.textures, sizeof(TextureResource) * resource_manager.texture_count);
    resource_manager.textures[resource_manager.texture_count - 1] = *texture;
}

static void add_subtexture(Subtexture *subtexture)
{
    // Placeholder implementation
}

static void add_animation(Animation *animation)
{
    // Placeholder implementation
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

        TraceLog(LOG_INFO, id);
        TraceLog(LOG_INFO, file);
        TraceLog(LOG_INFO, type);

        if (strcmp(type, "single") == 0)
        {
            // Load single texture
            TextureResource *texture = malloc(sizeof(TextureResource));
            texture->id = strdup(id);
            texture->texture = LoadTexture(file);
            // Add to resource manager (assume a function add_texture exists)
            add_texture(texture);
        }
        else if (strcmp(type, "atlas") == 0)
        {
            // Load texture atlas
            TextureResource *atlas_texture = malloc(sizeof(TextureResource));
            atlas_texture->id = id;
            atlas_texture->texture = LoadTexture(file);
            add_texture(atlas_texture);

            // Load subtextures
            JSON_Array *subtextures_array = json_object_get_array(texture_obj, "subtextures");
            for (size_t j = 0; j < json_array_get_count(subtextures_array); j++)
            {
                JSON_Object *subtexture_obj = json_array_get_object(subtextures_array, j);
                const char *id = json_object_get_string(subtexture_obj, "id");
                JSON_Object *rect_obj = json_object_get_object(subtexture_obj, "rect");

                Subtexture *subtexture = malloc(sizeof(Subtexture));
                subtexture->id = id;
                subtexture->rect.x = (float)json_object_get_number(rect_obj, "x");
                subtexture->rect.y = (float)json_object_get_number(rect_obj, "y");
                subtexture->rect.width = (float)json_object_get_number(rect_obj, "w");
                subtexture->rect.height = (float)json_object_get_number(rect_obj, "h");
                subtexture->texture_resource = atlas_texture;
                // Add subtexture to resource manager or texture atlas
                add_subtexture(subtexture);
            }

            // Load animations
            JSON_Array *animations_array = json_object_get_array(texture_obj, "animations");
            for (size_t j = 0; j < json_array_get_count(animations_array); j++)
            {
                JSON_Object *animation_obj = json_array_get_object(animations_array, j);
                const char *id = json_object_get_string(animation_obj, "id");
                int frame_count = (int)json_object_get_number(animation_obj, "frame_count");
                int framerate = (int)json_object_get_number(animation_obj, "framerate");

                Animation *animation = malloc(sizeof(Animation));
                animation->id = id;
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
                // Add animation to resource manager or texture atlas
                add_animation(animation);
            }
        }
    }

    json_value_free(root_value);
}

TextureResource *rm_get_texture(const char *id)
{
    TraceLog(LOG_INFO, "Searching for texture with id: %s", id);

    // Iterate through the textures array
    for (int i = 0; i < resource_manager.texture_count; i++)
    {
        TraceLog(LOG_INFO, "Comparing with texture id: %s", resource_manager.textures[i].id);

        // Compare the current texture's id with the provided id
        if (strcmp(resource_manager.textures[i].id, id) == 0)
        {
            TraceLog(LOG_INFO, "Texture found with id: %s", id);
            // If a match is found, return the pointer to the TextureResource
            return &resource_manager.textures[i];
        }
    }

    TraceLog(LOG_WARNING, "Texture with id '%s' not found", id);

    // If no match is found, return NULL
    return NULL;
}

Subtexture *rm_get_subtexture(const char *id)
{
    // Placeholder return
    return NULL;
}

Animation *rm_get_animation(const char *id)
{
    // Placeholder return
    return NULL;
}

void rm_cleanup(void)
{
    TraceLog(LOG_INFO, "rm_cleanup(void) called");

    // Free all textures
    for (int i = 0; i < resource_manager.texture_count; i++)
    {
        // Unload the texture from Raylib
        UnloadTexture(resource_manager.textures[i].texture);
    }
    free(resource_manager.textures); // Free the array of TextureResource

    free(resource_manager.subtextures); // Free the array of Subtexture

    // Free all animations and their frames
    for (int i = 0; i < resource_manager.animation_count; i++)
    {
        free(resource_manager.animations[i].subtexture_frames); // Free the array of Subtexture frames
    }
    free(resource_manager.animations); // Free the array of Animation

    // Reset counts and pointers to NULL
    resource_manager.texture_count = 0;
    resource_manager.subtexture_count = 0;
    resource_manager.animation_count = 0;

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
    .texture_count = 0,
    .subtextures = NULL,
    .subtexture_count = 0,
    .animations = NULL,
    .animation_count = 0};