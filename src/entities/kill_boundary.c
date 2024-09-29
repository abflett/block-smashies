#include <stdlib.h>
#include "kill_boundary.h"
#include "settings.h"
#include "collision_category.h"

static void clean_up_kill_boundary(KillBoundary *kill_boundary)
{
    TraceLog(LOG_INFO, "[Destroy] - Box2d KillBoundry [%d] - Success", kill_boundary->body.index1);
    b2DestroyBody(kill_boundary->body);
    free(kill_boundary);
}

KillBoundary *create_kill_boundary(b2WorldId world_id)
{
    KillBoundary *kill_boundary = (KillBoundary *)malloc(sizeof(KillBoundary));
    kill_boundary->type = ENTITY_KILL_BOUNDARY;

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_staticBody;
    kill_boundary->body = b2CreateBody(world_id, &bodyDef);

    Rectangle play_area = settings.game.play_area;
    b2Segment bottom_segment;
    bottom_segment.point1 = (b2Vec2){play_area.x - settings.gameplay.boundary_padding, settings.game.target_size.y - (play_area.y + play_area.height) - settings.gameplay.boundary_padding};
    bottom_segment.point2 = (b2Vec2){play_area.x + play_area.width + settings.gameplay.boundary_padding, settings.game.target_size.y - (play_area.y + play_area.height) - settings.gameplay.boundary_padding};

    b2ShapeDef segment_def = b2DefaultShapeDef();
    segment_def.friction = 0.0f;
    segment_def.restitution = 1.0f;

    segment_def.filter.categoryBits = CATEGORY_KILL_BOUNDARY;
    segment_def.filter.maskBits = KILL_BOUNDARY_COLLIDE_WITH;

    b2CreateSegmentShape(kill_boundary->body, &segment_def, &bottom_segment);

    kill_boundary->clean_up = clean_up_kill_boundary;

    b2Body_SetUserData(kill_boundary->body, kill_boundary);

    return kill_boundary;
}