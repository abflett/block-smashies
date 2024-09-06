#include "kill_boundary.h"
#include "game_settings.h"

#define BOUNDARY_PADDING 5.0f

static void clean_up_kill_boundary(KillBoundary *kill_boundary)
{
    b2DestroyBody(kill_boundary->body);
}

KillBoundary create_kill_boundary(b2WorldId world_id)
{
    KillBoundary kill_boundary;

    // Create a static body for the kill boundary
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_staticBody;
    kill_boundary.body = b2CreateBody(world_id, &bodyDef);

    Rectangle play_area = game_settings.play_area;
    b2Segment bottom_segment;
    bottom_segment.point1 = (b2Vec2){play_area.x - BOUNDARY_PADDING, game_settings.target_height - (play_area.y + play_area.height) - BOUNDARY_PADDING};
    bottom_segment.point2 = (b2Vec2){play_area.x + play_area.width + BOUNDARY_PADDING, game_settings.target_height - (play_area.y + play_area.height) - BOUNDARY_PADDING};

    // Define the shape properties (no density needed for static bodies)
    b2ShapeDef segment_def = b2DefaultShapeDef();
    segment_def.friction = 0.0f;
    segment_def.restitution = 1.0f; // Bouncy effect, if needed

    // Create the segment shape (kill boundary) attached to the body
    b2CreateSegmentShape(kill_boundary.body, &segment_def, &bottom_segment);

    // Assign the clean-up function
    kill_boundary.clean_up = clean_up_kill_boundary;

    return kill_boundary;
}