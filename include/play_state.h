#ifndef PLAY_STATE_H
#define PLAY_STATE_H

typedef struct PlayState
{
    void (*init)(void);
    void (*update)(float delta_time);
    void (*render)(void);
    void (*cleanup)(void);
} PlayState;

#endif