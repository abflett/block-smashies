#include "raylib.h"
#include "raymath.h"
#include "ext_collision.h"

static Edges rect_to_ext_edges(Rectangle rectangle, float radius)
{
    // Left Edge
    Edge leftEdge = (Edge){
        (Vector2){rectangle.x - radius, rectangle.y - radius},
        (Vector2){rectangle.x - radius, rectangle.y + rectangle.height + radius}};

    // Right Edge
    Edge rightEdge = (Edge){
        (Vector2){rectangle.x + rectangle.width + radius, rectangle.y - radius},
        (Vector2){rectangle.x + rectangle.width + radius, rectangle.y + rectangle.height + radius}};

    // Top Edge
    Edge topEdge = (Edge){
        (Vector2){rectangle.x - radius, rectangle.y - radius},
        (Vector2){rectangle.x + rectangle.width + radius, rectangle.y - radius}};

    // Bottom Edge
    Edge bottomEdge = (Edge){
        (Vector2){rectangle.x - radius, rectangle.y + rectangle.height + radius},
        (Vector2){rectangle.x + rectangle.width + radius, rectangle.y + rectangle.height + radius}};

    return (Edges){leftEdge, rightEdge, topEdge, bottomEdge};
}

static void check_collision_with_edge(CollisionResult *result, Vector2 line_start, Vector2 line_end, Edge edge, CollisionSide side)
{
    // float line_distance = Vector2Length(Vector2Subtract(line_start, line_end));
    // float edge_distance = Vector2Length(Vector2Subtract(edge.start, edge.end));
    // TraceLog(LOG_INFO, "line_distance: %f", line_distance);
    // TraceLog(LOG_INFO, "edge_distance: %f", edge_distance);

    Vector2 collisionPoint;
    if (CheckCollisionLines(line_start, line_end, edge.start, edge.end, &collisionPoint))
    {
        float distance = Vector2Length(Vector2Subtract(line_end, collisionPoint));
        if (distance < result->remaining_line)
        {
            result->collided = true;
            result->point = collisionPoint;
            result->remaining_line = Vector2Length(Vector2Subtract(line_end, collisionPoint));
            result->side = side;
        }
    }
}

CollisionResult check_collision_thick_line_rect(Vector2 line_start, Vector2 line_end, float radius, Rectangle rectangle)
{
    CollisionResult result = {false, {0, 0}, 360.0f, SIDE_NONE};
    Edges edges = rect_to_ext_edges(rectangle, radius);

    check_collision_with_edge(&result, line_start, line_end, edges.left, SIDE_LEFT);
    check_collision_with_edge(&result, line_start, line_end, edges.right, SIDE_RIGHT);
    check_collision_with_edge(&result, line_start, line_end, edges.top, SIDE_TOP);
    check_collision_with_edge(&result, line_start, line_end, edges.bottom, SIDE_BOTTOM);

    return result;
}
