#ifndef RESOURCE_H
#define RESOURCE_H

#include "raylib.h"
#include "uthash.h"

// Structure to represent a single texture resource
typedef struct
{
    const char *id;    // Unique identifier for the texture
    Texture2D texture; // Raylib texture object
    UT_hash_handle hh; // Handle for hash table management
} TextureResource;

// Structure to represent a subtexture within a texture atlas
typedef struct
{
    const char *id;                    // Name of the subtexture
    TextureResource *texture_resource; // Pointer to the texture resource
    Rectangle src;                     // Source rectangle defining the subtexture in the atlas
    UT_hash_handle hh;                 // Handle for hash table management
} Subtexture;

// Structure to represent an animation
typedef struct
{
    const char *id; // Name of the animation
    TextureResource *texture_resource;
    Rectangle *frames; // Array of textures that make up the animation
    int frame_count;
    UT_hash_handle hh; // Handle for hash table management
} Animation;

#endif // RESOURCE_H
