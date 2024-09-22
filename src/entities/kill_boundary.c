#include <stdlib.h>
#include "kill_boundary.h"
#include "settings.h"
#include "collision_category.h"

#define BOUNDARY_PADDING 4.0f

static void clean_up_kill_boundary(KillBoundary *kill_boundary)
{
    TraceLog(LOG_INFO, "[Cleanup] - KillBoundry [%d] - Success", kill_boundary->body.index1);
    b2DestroyBody(kill_boundary->body);
    free(kill_boundary);
}

KillBoundary *create_kill_boundary(b2WorldId world_id)
{
    KillBoundary *kill_boundary = (KillBoundary *)malloc(sizeof(KillBoundary));
    kill_boundary->type = ENTITY_KILL_BOUNDARY;
    // Create a static body for the kill boundary
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_staticBody;
    kill_boundary->body = b2CreateBody(world_id, &bodyDef);

    Rectangle play_area = settings.game.play_area;
    b2Segment bottom_segment;
    bottom_segment.point1 = (b2Vec2){play_area.x - BOUNDARY_PADDING, settings.game.target_size.y - (play_area.y + play_area.height) - BOUNDARY_PADDING};
    bottom_segment.point2 = (b2Vec2){play_area.x + play_area.width + BOUNDARY_PADDING, settings.game.target_size.y - (play_area.y + play_area.height) - BOUNDARY_PADDING};

    // Define the shape properties (no density needed for static bodies)
    b2ShapeDef segment_def = b2DefaultShapeDef();
    segment_def.friction = 0.0f;
    segment_def.restitution = 1.0f; // Bouncy effect, if needed

    segment_def.filter.categoryBits = CATEGORY_KILL_BOUNDARY;
    segment_def.filter.maskBits = KILL_BOUNDARY_COLLIDE_WITH; // Collide with everything except other balls

    // Create the segment shape (kill boundary) attached to the body
    b2CreateSegmentShape(kill_boundary->body, &segment_def, &bottom_segment);

    // Assign the clean-up function
    kill_boundary->clean_up = clean_up_kill_boundary;

    b2Body_SetUserData(kill_boundary->body, kill_boundary);

    return kill_boundary;
}