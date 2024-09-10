#ifndef ENTITY_TYPES_H
#define ENTITY_TYPES_H

typedef enum EntityType
{
    ENTITY_NONE,
    ENTITY_BALL,
    ENTITY_BRICK,
    ENTITY_PADDLE,
    ENTITY_NANITE,
    ENTITY_WALL,
    ENTITY_KILL_BOUNDARY
} EntityType;

#endif