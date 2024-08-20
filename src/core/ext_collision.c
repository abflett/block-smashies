#include "raylib.h"
#include "raymath.h"
#include "ext_collision.h"

static Edges rect_to_ext_edges(Rectangle rectangle, float radius)
{
    // Left Edge
    Edge leftEdge = (Edge){
        (Vector2){rectangle.x, rectangle.y - radius},
        (Vector2){rectangle.x, rectangle.y + rectangle.height + radius}};

    // Right Edge
    Edge rightEdge = (Edge){
        (Vector2){rectangle.x + rectangle.width, rectangle.y - radius},
        (Vector2){rectangle.x + rectangle.width, rectangle.y + rectangle.height + radius}};

    // Top Edge
    Edge topEdge = (Edge){
        (Vector2){rectangle.x - radius, rectangle.y},
        (Vector2){rectangle.x + rectangle.width + radius, rectangle.y}};

    // Bottom Edge
    Edge bottomEdge = (Edge){
        (Vector2){rectangle.x - radius, rectangle.y + rectangle.height},
        (Vector2){rectangle.x + rectangle.width + radius, rectangle.y + rectangle.height}};

    return (Edges){leftEdge, rightEdge, topEdge, bottomEdge};
}

static void check_collision_with_edge(CollisionResult *result, Vector2 line_start, Vector2 line_end, Edge edge, CollisionSide side)
{
    Vector2 collisionPoint;
    if (CheckCollisionLines(line_start, line_end, edge.start, edge.end, &collisionPoint))
    {
        float distance = Vector2Length(Vector2Subtract(line_start, collisionPoint));
        if (distance < result->remaining_line)
        {
            result->collided = true;
            result->point = collisionPoint;
            result->remaining_line = Vector2Length(Vector2Subtract(line_end, collisionPoint));
            result->side = side;
        }
    }
}

CollisionResult check_collision_thick_line_rect(Vector2 line_start, Vector2 line_end, float radius, Rectangle rect)
{
    CollisionResult result = {false, {0, 0}, 2000.0f, SIDE_NONE};
    Edges edges = rect_to_ext_edges(rect, radius);

    check_collision_with_edge(&result, line_start, line_end, edges.left, SIDE_LEFT);
    check_collision_with_edge(&result, line_start, line_end, edges.right, SIDE_RIGHT);
    check_collision_with_edge(&result, line_start, line_end, edges.top, SIDE_TOP);
    check_collision_with_edge(&result, line_start, line_end, edges.bottom, SIDE_BOTTOM);

    return result;
}
