#ifndef RESOURCE_H
#define RESOURCE_H

#include "raylib.h"

// Structure to represent a single texture resource
typedef struct
{
    const char *id;    // Unique identifier for the texture
    Texture2D texture; // Raylib texture object
} TextureResource;

// Structure to represent a subtexture within a texture atlas
typedef struct
{
    const char *id;                    // Name of the subtexture
    Rectangle rect;                    // Source rectangle defining the subtexture in the atlas
    TextureResource *texture_resource; // Pointer to the texture texture resource
} Subtexture;

// Structure to represent an animation
typedef struct
{
    const char *id;                // Name of the animation
    Subtexture *subtexture_frames; // Array of subtextures that make up the animation
    int frame_count;               // Number of frames in the animation
    int framerate;                 // Framerate of the animation
} Animation;

#endif // RESOURCE_H
