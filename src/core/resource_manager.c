#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "uthash.h"
#include "raylib.h"
#include "parson.h"
#include "resource_manager.h"
#include "settings.h"

static void rm_load_resource_file(void)
{
    // Parse the JSON file
    JSON_Value *root_value = json_parse_file(settings.file_locations.resource_file);
    JSON_Object *root_object = json_value_get_object(root_value);
    JSON_Array *textures_array = json_object_get_array(root_object, "textures");
    JSON_Array *subtextures_array = json_object_get_array(root_object, "subtextures");
    JSON_Array *animations_array = json_object_get_array(root_object, "animations");

    const char *pixel7_font = json_object_get_string(root_object, "font");
    resource_manager.pixel7_font = LoadFont(pixel7_font);

    resource_manager.brick_type_mapper = create_brick_type_mapper(root_object);
    resource_manager.bar_level_mapper = create_bar_level_mapper(root_object);
    resource_manager.nanite_type_mapper = create_nanite_type_mapper(root_object);
    resource_manager.ship_color_mapper = create_ship_color_mapper(root_object);
    resource_manager.shield_type_mapper = create_shield_type_mapper(root_object);
    resource_manager.thruster_level_mapper = create_thruster_level_mapper(root_object);
    resource_manager.node_perk_mapper = create_node_perk_mapper(root_object);
    resource_manager.node_attribute_mapper = create_node_attribute_mapper(root_object);
    resource_manager.upgrade_icon_mapper = create_upgrade_icon_mapper(root_object);

    for (size_t i = 0; i < (int)json_array_get_count(textures_array); i++)
    {
        JSON_Object *texture_obj = json_array_get_object(textures_array, i);
        const char *id = json_object_get_string(texture_obj, "id");
        const char *file = json_object_get_string(texture_obj, "file");

        TextureResource *texture = malloc(sizeof(TextureResource));
        texture->id = strdup(id); // Copy string to avoid dangling pointer
        texture->texture = LoadTexture(file);
        HASH_ADD_KEYPTR(hh, resource_manager.textures, texture->id, strlen(texture->id), texture);
    }

    for (size_t i = 0; i < (int)json_array_get_count(subtextures_array); i++)
    {
        JSON_Object *subtexture_obj = json_array_get_object(subtextures_array, i);
        JSON_Object *src_rect_obj = json_object_get_object(subtexture_obj, "src");
        const char *id = json_object_get_string(subtexture_obj, "id");
        const char *texture_id = json_object_get_string(subtexture_obj, "texture");
        const char *subtexture_type = json_object_get_string(subtexture_obj, "type");

        Subtexture *subtexture = malloc(sizeof(Subtexture));
        subtexture->id = strdup(id); // Copy string to avoid dangling pointer
        subtexture->texture_resource = resource_manager.get_texture(texture_id);
        subtexture->src.x = (float)json_object_get_number(src_rect_obj, "x");
        subtexture->src.y = (float)json_object_get_number(src_rect_obj, "y");
        subtexture->src.width = (float)json_object_get_number(src_rect_obj, "w");
        subtexture->src.height = (float)json_object_get_number(src_rect_obj, "h");

        HASH_ADD_KEYPTR(hh, resource_manager.subtextures, subtexture->id, strlen(subtexture->id), subtexture);
    }

    // Iterate anitmations
    for (size_t i = 0; i < (int)json_array_get_count(animations_array); i++)
    {
        JSON_Object *animation_obj = json_array_get_object(animations_array, i);
        JSON_Array *frames_array = json_object_get_array(animation_obj, "frames");
        const char *id = json_object_get_string(animation_obj, "id");
        const char *texture_id = json_object_get_string(animation_obj, "texture");

        Animation *animation = malloc(sizeof(Animation));
        animation->id = strdup(id);
        animation->texture_resource = resource_manager.get_texture(texture_id);
        animation->frame_count = (int)json_array_get_count(frames_array);
        animation->frames = malloc(animation->frame_count * sizeof(Rectangle));

        for (int k = 0; k < animation->frame_count; k++)
        {
            JSON_Object *frame_rect_obj = json_array_get_object(frames_array, k);

            Rectangle frame;
            frame.x = (float)json_object_get_number(frame_rect_obj, "x");
            frame.y = (float)json_object_get_number(frame_rect_obj, "y");
            frame.width = (float)json_object_get_number(frame_rect_obj, "w");
            frame.height = (float)json_object_get_number(frame_rect_obj, "h");

            animation->frames[k] = frame;
        }

        HASH_ADD_KEYPTR(hh, resource_manager.animations, animation->id, strlen(animation->id), animation);
    }

    json_value_free(root_value);
}

static TextureResource *rm_get_texture(const char *id)
{
    TextureResource *texture = NULL;
    HASH_FIND_STR(resource_manager.textures, id, texture);
    return texture;
}

static Subtexture *rm_get_subtexture(const char *id)
{
    Subtexture *subtexture = NULL;
    HASH_FIND_STR(resource_manager.subtextures, id, subtexture);
    return subtexture;
}

static Animation *rm_get_animation(const char *id)
{
    Animation *animation = NULL;
    HASH_FIND_STR(resource_manager.animations, id, animation);
    return animation;
}

static Font *rm_get_pixel7_font(void)
{
    return &resource_manager.pixel7_font;
}

static void rm_cleanup(void)
{
    // Free all subtextures
    Subtexture *subtexture, *tmp_subtexture;
    HASH_ITER(hh, resource_manager.subtextures, subtexture, tmp_subtexture)
    {
        HASH_DEL(resource_manager.subtextures, subtexture);
        free((char *)subtexture->id); // Free duplicated string
        free(subtexture);
    }
    TraceLog(LOG_INFO, "[Cleanup] - Subtextures - Success");

    // Free all animations and their frames
    Animation *animation, *tmp_animation;
    HASH_ITER(hh, resource_manager.animations, animation, tmp_animation)
    {
        free(animation->frames);
        HASH_DEL(resource_manager.animations, animation);
        free((char *)animation->id); // Free duplicated string
        free(animation);
    }
    TraceLog(LOG_INFO, "[Cleanup] - Animations - Success");

    // Free all textures
    TextureResource *texture, *tmp_texture;
    HASH_ITER(hh, resource_manager.textures, texture, tmp_texture)
    {
        UnloadTexture(texture->texture);
        HASH_DEL(resource_manager.textures, texture);
        free((char *)texture->id); // Free duplicated string
        free(texture);
    }
    TraceLog(LOG_INFO, "[Cleanup] - Textures - Success");

    resource_manager.subtextures = NULL;
    resource_manager.animations = NULL;
    resource_manager.textures = NULL;

    // cleanup mappers
    resource_manager.brick_type_mapper->cleanup();
    resource_manager.bar_level_mapper->cleanup();
    resource_manager.nanite_type_mapper->cleanup();
    resource_manager.shield_type_mapper->cleanup();
    resource_manager.ship_color_mapper->cleanup();
    resource_manager.thruster_level_mapper->cleanup();
    resource_manager.node_perk_mapper->cleanup();
    resource_manager.node_attribute_mapper->cleanup();
    resource_manager.upgrade_icon_mapper->cleanup();

    UnloadFont(resource_manager.pixel7_font);
}

ResourceManager resource_manager = {
    .load_resource_file = rm_load_resource_file,
    .get_texture = rm_get_texture,
    .get_subtexture = rm_get_subtexture,
    .get_animation = rm_get_animation,
    .get_pixel7_font = rm_get_pixel7_font,
    .cleanup = rm_cleanup,
    .textures = NULL,
    .subtextures = NULL,
    .animations = NULL,
    .pixel7_font = {0}};