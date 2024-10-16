#include "ship.h"
#include "entity_type.h"
#include "game_context.h"
#include "collision_category.h"

static int calculate_segments_func(Ship *ship)
{
    TraceLog(LOG_INFO, "Player: %d, Count: %d", *ship->player, *ship->player_count);
    ship->previous_player_count = *ship->player_count;
    switch (*ship->player_count)
    {
    case 1:
        return 4; // Single player, 4 segments
    case 2:
        return 2; // 2 players, each with 2 segments
    case 3:
        return (*ship->player == 0) ? 2 : 1; // Player 1 has 2, others have 1
    case 4:
        return 1; // 4 players, each with 1 segment
    default:
        return 1; // Fallback, should not happen unless player_count is misconfigured
    }
}

// For static animations, velocity will show correct thruster animation
static void move(Ship *ship, b2Vec2 position, b2Vec2 velocity)
{
    ship->velocity = velocity;
    ship->position = position;
}

static void update_ship(Ship *ship, float delta_time)
{
    // boost cooldown timer
    if (ship->boost_active_timer > 0)
    {
        ship->boost_active_timer -= delta_time;
    }

    // pulse cooldown timer
    if (ship->pulse_active_timer > 0)
    {
        ship->pulse_active_timer -= delta_time;
    }

    // pulse animation timer
    if (ship->pulse_timer > 0)
    {
        ship->pulse_timer -= delta_time;
    }

    // return paddle back to X constraint after pulse
    if (ship->pulse_timer <= 0.0f && !b2Body_IsEnabled(ship->constraint))
    {
        b2Body_Enable(ship->constraint);
    }

    ship->velocity = b2Body_GetLinearVelocity(ship->body);
}

static void render_ship(Ship *ship)
{
    if (ship->physics_active)
    {
        ship->position = b2Body_GetPosition(ship->body);
    }

    ship->position = (b2Vec2){ship->position.x, (float)(int)ship->position.y};

    if (ship->previous_player_count != *ship->player_count)
    {
        ship->segments = calculate_segments_func(ship);
    }
    ship->ship_body->render(ship->ship_body);
    ship->ship_shield->render(ship->ship_shield);
    ship->ship_thrusters->render(ship->ship_thrusters);
}

static void disable_ship(Ship *ship) {}

static void reset_ship(Ship *ship, b2Vec2 position) {}

static void cleanup_ship(Ship *ship)
{
    ship->ship_body->cleanup(ship->ship_body);
    ship->ship_shield->cleanup(ship->ship_shield);
    ship->ship_thrusters->cleanup(ship->ship_thrusters);

    if (ship->physics_active)
    {
        b2DestroyBody(ship->body);
        b2DestroyBody(ship->constraint);
    }

    free(ship);
}

static void activate_ship_physics(Ship *ship, GameContext *game_context)
{
    TraceLog(LOG_INFO, "Activating ship physics");
    b2BodyDef body_def = b2DefaultBodyDef();
    body_def.type = b2_dynamicBody;
    body_def.position = ship->position; // set position to ship position
    body_def.linearDamping = 3.0f;
    body_def.isBullet = true;
    ship->body = b2CreateBody(game_context->world_id, &body_def);

    // set physics body size in relation to the shield
    b2Polygon shield_box = b2MakeBox(ship->shield_size.x * 0.5f, ship->shield_size.y);

    b2ShapeDef shield_shape_def = b2DefaultShapeDef();
    shield_shape_def.density = 1.0f;
    shield_shape_def.friction = 0.1f;
    shield_shape_def.restitution = 1.0f; // High restitution for bouncing

    shield_shape_def.filter.categoryBits = CATEGORY_PADDLE;
    shield_shape_def.filter.maskBits = PADDLE_COLLIDE_WITH;

    b2CreatePolygonShape(ship->body, &shield_shape_def, &shield_box);

    // Create a static reference body for the prismatic joint
    b2BodyDef static_body_def = b2DefaultBodyDef();
    static_body_def.type = b2_staticBody;
    static_body_def.position = (b2Vec2){(settings.game.play_area.width / 2) + settings.game.play_area.x, 19.0f}; // Static reference point
    ship->constraint = b2CreateBody(game_context->world_id, &static_body_def);

    // Prismatic Joint: Constrain movement to the X-axis
    b2PrismaticJointDef x_joint_def = b2DefaultPrismaticJointDef();
    x_joint_def.bodyIdA = ship->body;
    x_joint_def.bodyIdB = ship->constraint;
    x_joint_def.localAxisA = (b2Vec2){1.0f, 0.0f}; // Constrain to X-axis
    x_joint_def.enableLimit = false;               // No strict limits along X-axis
    x_joint_def.enableMotor = false;
    b2CreatePrismaticJoint(game_context->world_id, &x_joint_def);

    b2MassData mass_data = b2Body_GetMassData(ship->body);
    mass_data.mass = settings.gameplay.paddle_mass;
    b2Body_SetMassData(ship->body, mass_data);

    ship->physics_active = true;
    b2Body_SetUserData(ship->body, ship);
}

// Ship input commands
static void move_ship(Ship *ship, int direction)
{
    b2Body_ApplyForceToCenter(ship->body, (b2Vec2){direction * (*ship->force), 0.0f}, true);
}

static void boost_ship(Ship *ship, int direction)
{
    if (ship->boost_active_timer <= 0.0f)
    {
        b2Body_ApplyLinearImpulse(ship->body, (b2Vec2){direction * (*ship->boost_force), 0.0f}, b2Body_GetWorldCenterOfMass(ship->body), true);
        ship->boost_active_timer = *ship->boost_cooldown;
    }
}

static void pulse_ship(Ship *ship)
{
    if (ship->pulse_active_timer <= 0.0f)
    {
        b2Body_Disable(ship->constraint);
        b2Body_ApplyLinearImpulse(ship->body, (b2Vec2){0.0f, *ship->pulse_force}, b2Body_GetWorldCenterOfMass(ship->body), true);

        ship->pulse_timer = settings.gameplay.pulse_timer; // pulse animation up timer
        ship->pulse_active_timer = *ship->pulse_cooldown;
    }
}

Ship *create_ship(int *player, GameData *game_data, b2Vec2 position)
{
    Ship *ship = malloc(sizeof(Ship));

    ship->type = ENTITY_PADDLE;
    ship->active = false;
    ship->physics_active = false;
    ship->player = player;
    ship->player_count = &game_data->player_count;
    ship->previous_player_count = *ship->player_count;
    ship->ship_color = &game_data->ships[*ship->player].ship_color;

    ship->position = position;
    ship->velocity = (b2Vec2){0, 0};

    // current timers
    ship->pulse_timer = 0.0f;
    ship->pulse_active_timer = 0.0f;
    ship->boost_active_timer = 0.0f;

    // perks
    ship->phase_shift = &game_data->perks.phase_shift;
    ship->orb_shot = &game_data->perks.orb_shot;

    // new paddle attributes
    ship->force = &game_data->paddle.force;                   // general movement force
    ship->friction = &game_data->paddle.friction;             // ball manipulation
    ship->damping = &game_data->paddle.damping;               // de-acceleration - affects max velocity as well
    ship->max_energy = &game_data->paddle.max_energy;         // max_energy
    ship->boost_force = &game_data->paddle.boost_force;       // boost force - horizontal burst
    ship->boost_cooldown = &game_data->paddle.boost_cooldown; // boost cooldown timer < is better
    ship->pulse_force = &game_data->paddle.pulse_force;       // boost force - vertical burst
    ship->pulse_cooldown = &game_data->paddle.pulse_cooldown; // pulse cooldown timer < is better
    ship->heat = &game_data->paddle.max_heat;                 // heat buildup % < is no heat

    ship->shield_level = &game_data->ships[*ship->player].shield_level;
    ship->segments = calculate_segments_func(ship);

    ship->ship_body = create_ship_body(&ship->segments, ship->ship_color, &ship->position);
    ship->ship_shield = create_ship_shield(&ship->segments, ship->shield_level, &ship->position);
    ship->ship_thrusters = create_ship_thrusters(&ship->segments, &ship->position);

    ship->shield_size = (b2Vec2){ship->ship_shield->subtexture->src.width, ship->ship_shield->subtexture->src.height};

    ship->move_ship = move_ship;
    ship->boost_ship = boost_ship;
    ship->pulse_ship = pulse_ship;

    ship->activate_ship_physics = activate_ship_physics;
    ship->calculate_segments = calculate_segments_func;
    ship->move = move;
    ship->update = update_ship;
    ship->render = render_ship;
    ship->disable = disable_ship;
    ship->reset = reset_ship;
    ship->cleanup = cleanup_ship;

    return ship;
}
