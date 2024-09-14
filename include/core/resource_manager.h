#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "kvec.h"
#include "resource.h"

typedef struct ResourceManager
{
    kvec_t(BrickSubtextureIdsSet) brick_type_sets;
    kvec_t(BrickAnimationId) brick_animation_ids;
    // Function pointers for managing resources
    void (*load_resource_file)(const char *file);    // Load the JSON file and store resources in memory
    TextureResource *(*get_texture)(const char *id); // Get a texture by its ID
    Subtexture *(*get_subtexture)(const char *id);   // Get a subtexture by its ID
    Animation *(*get_animation)(const char *id);     // Get an animation by its ID
    void (*cleanup)(void);                           // Clean up all loaded resources

    // Internal storage using hash tables for fast lookups
    TextureResource *textures; // Hash table of textures
    Subtexture *subtextures;   // Hash table of subtextures
    Animation *animations;     // Hash table of animations

} ResourceManager;

// Declare an extern instance of ResourceManager
extern ResourceManager resource_manager;

#endif // RESOURCE_MANAGER_H
