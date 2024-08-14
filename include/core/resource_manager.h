#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "resource.h"

typedef struct ResourceManager
{
    // Function pointers for managing resources
    void (*load_resource_file)(const char *file);    // Load the JSON file and store resources in memory
    TextureResource *(*get_texture)(const char *id); // Get a texture by its ID
    Subtexture *(*get_subtexture)(const char *id);   // Get a subtexture by its ID
    Animation *(*get_animation)(const char *id);     // Get an animation by its ID
    void (*cleanup)(void);                           // Clean up all loaded resources

    // Internal storage (e.g., arrays or hash maps for resources)
    TextureResource *textures;
    int texture_count;
    Subtexture *subtextures;
    int subtexture_count;
    Animation *animations;
    int animation_count;
    // Add other resource types as needed

} ResourceManager;

static void add_texture(TextureResource *texture);
static void add_subtexture(Subtexture *subtexture);
static void add_animation(Animation *animation);

// Declare an extern instance of ResourceManager
extern ResourceManager resource_manager;
#endif