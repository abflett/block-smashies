#include <stdlib.h>
#include "wall_edges.h"
#include "settings.h" // To access settings.game.play_area

static void clean_up_edges(WallEdges *walls)
{
    TraceLog(LOG_INFO, "[Destroy] - Box2d WallEdges [%d] - Success", walls->body.index1);
    b2DestroyBody(walls->body);
    free(walls);
}

WallEdges *create_wall_edges(const b2WorldId world_id)
{
    WallEdges *walls = (WallEdges *)malloc(sizeof(WallEdges));
    walls->type = ENTITY_WALL;

    // Create a static body for the walls
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_staticBody;
    walls->body = b2CreateBody(world_id, &bodyDef);

    // Define the vertices of the walls (left, top, right) based on play_area from game_settings
    Rectangle play_area = settings.game.play_area;
    b2Vec2 vertices[4];
    vertices[0] = (b2Vec2){play_area.x, settings.game.target_size.y - (play_area.y + play_area.height) - settings.gameplay.edge_padding};                   // Bottom-left (left wall start)
    vertices[1] = (b2Vec2){play_area.x, settings.game.target_size.y - play_area.y};                                                                         // Top-left (left wall end, top wall start)
    vertices[2] = (b2Vec2){play_area.x + play_area.width, settings.game.target_size.y - play_area.y};                                                       // Top-right (top wall end, right wall start)
    vertices[3] = (b2Vec2){play_area.x + play_area.width, settings.game.target_size.y - (play_area.y + play_area.height) - settings.gameplay.edge_padding}; // Bottom-right (right wall end)
    int count = sizeof(vertices) / sizeof(vertices[0]);

    // Define the chain shape with the vertices
    b2ChainDef chain = b2DefaultChainDef();
    chain.points = vertices;
    chain.isLoop = true;
    chain.count = count;
    chain.friction = 0.0f;
    chain.restitution = 1.0f;

    b2CreateChain(walls->body, &chain);

    walls->clean_up = clean_up_edges;

    b2Body_SetUserData(walls->body, walls);

    return walls;
}
