#ifndef SCENE_H
#define SCENE_H

typedef struct Scene
{
    void (*init)(void);
    void (*update)(float delta_time);
    void (*render)(void);
    void (*cleanup)(void);
} Scene;

#endif
