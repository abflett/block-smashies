#include <stdlib.h>
#include "entities.h"
#include "ball.h"
#include "paddle.h"

// Function to add a Ball to the Entities
void add_ball_func(Entities *entities, Ball ball) {
    kv_push(Ball, entities->balls, ball); // Add the ball to the balls vector
}

// Function to add a Paddle to the Entities
void add_paddle_func(Entities *entities, Paddle paddle) {
    kv_push(Paddle, entities->paddles, paddle); // Add the paddle to the paddles vector
}

// Function to remove a Ball by index
void remove_ball_func(Entities *entities, int index) {
    if (index >= 0 && index < kv_size(entities->balls)) {
        kv_A(entities->balls, index).active = false; // Mark as inactive
        // Alternatively, remove the element completely if needed
    }
}

// Function to remove a Paddle by index
void remove_paddle_func(Entities *entities, int index) {
    if (index >= 0 && index < kv_size(entities->paddles)) {
        kv_A(entities->paddles, index).active = false; // Mark as inactive
        // Alternatively, remove the element completely if needed
    }
}

// Function to update all entities
void update_entities_func(Entities *entities, float delta_time) {
    for (int i = 0; i < kv_size(entities->balls); i++) {
        Ball *ball = &kv_A(entities->balls, i);
        if (ball->active) {
            ball->update(ball, entities, delta_time);
        }
    }

    for (int i = 0; i < kv_size(entities->paddles); i++) {
        Paddle *paddle = &kv_A(entities->paddles, i);
        if (paddle->active) {
            paddle->update(paddle, delta_time);
        }
    }
}

// Function to render all entities
void render_entities_func(Entities *entities) {
    for (int i = 0; i < kv_size(entities->balls); i++) {
        Ball *ball = &kv_A(entities->balls, i);
        if (ball->active) {
            ball->render(ball);
        }
    }

    for (int i = 0; i < kv_size(entities->paddles); i++) {
        Paddle *paddle = &kv_A(entities->paddles, i);
        if (paddle->active) {
            paddle->render(paddle);
        }
    }
}

// Function to clean up all entities
void cleanup_entities_func(Entities *entities) {
    // Clean up balls
    kv_destroy(entities->balls); // Free the memory allocated for the balls vector

    // Clean up paddles
    kv_destroy(entities->paddles); // Free the memory allocated for the paddles vector
}

// Function to create and initialize the Entities struct
Entities create_entities() {
    Entities entities;
    kv_init(entities.balls);
    kv_init(entities.paddles);

    // Assign function pointers
    entities.add_ball = add_ball_func;
    entities.add_paddle = add_paddle_func;
    entities.remove_ball = remove_ball_func;
    entities.remove_paddle = remove_paddle_func;
    entities.update = update_entities_func;
    entities.render = render_entities_func;
    entities.cleanup = cleanup_entities_func;  // Assign cleanup function

    return entities;
}
