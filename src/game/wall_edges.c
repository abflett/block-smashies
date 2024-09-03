#include "wall_edges.h"
#include "box2d/box2d.h"

static void create_edge(b2WorldId world_id, b2BodyId *body, b2Vec2 start, b2Vec2 end)
{
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_staticBody;
    *body = b2CreateBody(world_id, &bodyDef);

    // Define the edge shape
    b2Segment edgeSegment;
    edgeSegment.point1 = start;
    edgeSegment.point2 = end;

    b2ShapeDef edge_def = b2DefaultShapeDef();
    edge_def.density = 0.0f;
    edge_def.friction = 0.0f;

    b2CreateEdgeShape(*body, &edge_def, &edgeSegment);
}

static void clean_up_edges(struct WallEdges *walls)
{
    b2DestroyBody(walls->left_wall);
    b2DestroyBody(walls->right_wall);
    b2DestroyBody(walls->top_wall);
}

WallEdges create_wall_edges(b2WorldId world_id, Rectangle play_area)
{
    WallEdges walls;
    walls.bounds = play_area;

    // Create the left, right, and top edges
    create_edge(world_id, &walls.left_wall,
                (b2Vec2){play_area.x, play_area.y},
                (b2Vec2){play_area.x, play_area.y + play_area.height});

    create_edge(world_id, &walls.right_wall,
                (b2Vec2){play_area.x + play_area.width, play_area.y},
                (b2Vec2){play_area.x + play_area.width, play_area.y + play_area.height});

    create_edge(world_id, &walls.top_wall,
                (b2Vec2){play_area.x, play_area.y},
                (b2Vec2){play_area.x + play_area.width, play_area.y});

    walls.clean_up = clean_up_edges;

    return walls;
}
