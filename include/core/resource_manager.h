#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "raylib.h"
#include "kvec.h"
#include "resource.h"
#include "brick_type_mapper.h"
#include "bar_level_mapper.h"
#include "nanite_type_mapper.h"

typedef struct ResourceManager
{
    BrickTypeMapper *brick_type_mapper;
    BarLevelMapper *bar_level_mapper;
    NaniteTypeMapper *nanite_type_mapper;
    // Function pointers for managing resources
    void (*load_resource_file)(const char *file);    // Load the JSON file and store resources in memory
    TextureResource *(*get_texture)(const char *id); // Get a texture by its ID
    Subtexture *(*get_subtexture)(const char *id);   // Get a subtexture by its ID
    Animation *(*get_animation)(const char *id);     // Get an animation by its ID
    Font *(*get_pixel7_font)(void);
    void (*cleanup)(void); // Clean up all loaded resources

    Font pixel7_font;
    // Internal storage using hash tables for fast lookups
    TextureResource *textures; // Hash table of textures
    Subtexture *subtextures;   // Hash table of subtextures
    Animation *animations;     // Hash table of animations

} ResourceManager;

// Declare an extern instance of ResourceManager
extern ResourceManager resource_manager;

#endif // RESOURCE_MANAGER_H
