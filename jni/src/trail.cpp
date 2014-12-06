#include "trail.h"
#include "log.h"
#include "box.h"
#include "game.h"

Trail::Trail() {
    _boxes_length = 20;
}

Trail::~Trail() {
    for(int i=0; i<(int)shapes.size(); i++) {
        // Clear boxes array
        std::vector<Shape>().swap(shapes);
    }
}

void Trail::setup() {
    _renderer.setup();
}

void Trail::fade(Shape &shape) {
    // fade the box when it has stopped moving vertically
    float fade_decr = 0.02f;    // Larger = faster fade
    if (shape.getAlpha() >= fade_decr) {
        shape.setAlpha(shape.getAlpha() - fade_decr);
    }
    else if (shape.getAlpha() < fade_decr && shape.getAlpha() > 0.0f)
        shape.setAlpha(0.0f);
}

void Trail::render(PhysicsEngine &physics) {
    // Draw every shape in shapes vector
    for (int i=0; i<(int)shapes.size(); i++) {
        if (shapes.at(i).vert_motion.getVel() == 0)
            fade(shapes.at(i));

        // Send shape to renderer
        _renderer.renderShape(&shapes.at(i));

        // Update physics attributes only if box is moving
        if (shapes.at(i).vert_motion.getVel() != 0.0f || shapes.at(i).hori_motion.getVel() != 0.0f)
            physics.updatePhysics(shapes.at(i), Game::getElapsedTime(), Game::getScreenWidth(), Game::getScreenHeight());

        // Remove shape if no longer needed
        if (shapes.at(i).getAlpha() <= 0 && (int)shapes.size() > 0 && shapes.at(i).vert_motion.getVel() == 0) {
            removeBox(i);
            // shapes.erase(shapes.begin() + i);
            // LOGI("remove_box() -> removed %i", i);
            i--;
        }
    }

    _renderer.disableAttributes();
}

void Trail::buildTrail(float x, float y, Theme theme, PhysicsEngine &physics) {
    // Center position
    x -= _boxes_length/2;
    y -= _boxes_length/2;

    /* Increase size of array and check for exceptions */
    try {
        // shapes.resize(shapes.size() + 1, Box(x, y, _boxes_length, (int)shapes.size(), time, theme));
        shapes.push_back(Box(x, y, _boxes_length, theme));
    }
    catch (std::exception &e) {
        LOGE("Error occured while creating box: %s", e.what());
    }

    LOGI("create_box() -> created %i", (int)shapes.size());

    /* Setup new box */
    Shape *shape = &shapes[(int)shapes.size() - 1];

    // Create the fountain effect
    physics.generateInitVelocity(*shape, shape->rot_angle);

    // Just for testing
    // box->hori_motion.setAccel(-1.3f);

    // Allow gravity to affect the box
    shape->vert_motion.setAccel(physics.getGravity());
    // box->vert_motion.setAccel(-0.1f);
    
    // box->rot_angle = box->hori_motion.getVel() + box->vert_motion.getVel();
}


void Trail::removeBox(int index) {
    // Shift box positions
    if (index < (int)shapes.size() - 1) {
        for (int i = index; i < (int)shapes.size(); i++) {
            shapes[i] = shapes[i + 1];
        }
    }

    // Decrease size of array & realloc with less mem
    // shapes.resize((int)shapes.size() - 1);
    shapes.erase(shapes.begin() + (index));
    LOGI("remove_box() -> removed %i", index);
}


void Trail::rotate(float angle) {
    float initial_vel_vert = 0.0f;
    float initial_vel_hori = 0.0f;

    if (angle >= 0 && angle < 90) {
        for (int i=0; i<(int)shapes.size(); i++) {
            shapes.at(i).hori_motion.setVel(shapes.at(i).hori_motion.getVel() + angle);
        }
    }
}
