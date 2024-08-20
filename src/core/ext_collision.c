// ext_collision.c
#include "raylib.h"
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

static void point_line(PointLine *result, Vector2 line_start, Vector2 line_end, Vector2 edge_start, Vector2 edge_end, float *closestDistance)
{
    Vector2 collisionPoint;

    if (check_collision_lines(line_start, line_end, edge_start, edge_end, &collisionPoint))
    {
        float distance = Vector2Length(Vector2Subtract(line_start, collisionPoint));
        if (distance < *closestDistance)
        {
            *closestDistance = distance;
            result->collision = true;
            result->point = collisionPoint;
            result->line = Vector2Length(Vector2Subtract(line_end, collisionPoint));
        }
    }
}

PointLine check_collision_thick_line_rect(Vector2 line_start, Vector2 line_end, float radius, Rectangle rect)
{
    PointLine result = {false, {0, 0}, -1.0f};
    Edges edges = rect_to_ext_edges(rect, radius);
    float closestDistance = 1920.0f; // Set to a large number initially

    point_line(&result, line_start, line_end, edges.left.start, edges.left.end, &closestDistance);
    point_line(&result, line_start, line_end, edges.right.start, edges.right.end, &closestDistance);
    point_line(&result, line_start, line_end, edges.top.start, edges.top.end, &closestDistance);
    point_line(&result, line_start, line_end, edges.bottom.start, edges.bottom.end, &closestDistance);

    return result;
}
