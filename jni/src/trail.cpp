#include "trail.h"
#include "log.h"

Trail::Trail() {
    _boxes_length = 20;
}

void Trail::setup(int screen_w, int screen_h) {
    _renderer.setup(screen_w, screen_h);
}

void Trail::fade(Box &box) {
    // fade the box when it has stopped moving vertically
    float fade_decr = 0.04f;    // Larger = faster fade
    if (box.vert_motion.getVel() == 0 && box.getAlpha() >= fade_decr)
        box.setAlpha(box.getAlpha() - fade_decr);
    if (box.getAlpha() < fade_decr && box.getAlpha() > 0.0f)
        box.setAlpha(0.0f);
}

void Trail::render(PhysicsEngine &physics, int time, int screen_width, int screen_height) {
    // Render changes
    _renderer.renderFrame();

    // Draw every shape in shapes vector
    for (int i=0; i<(int)shapes.size(); i++) {
        fade(shapes.at(i));

        // Send shape to renderer
        _renderer.renderBox(shapes.at(i));

        // Update physics attributes only if box is moving
        if (shapes.at(i).vert_motion.getVel() != 0.0f || shapes.at(i).hori_motion.getVel() != 0.0f)
            physics.updatePhysics(shapes.at(i), time, screen_width, screen_height);

        // Remove shape if no longer needed
        if (shapes.at(i).getAlpha() <= 0 && (int)shapes.size() > 0 && shapes.at(i).vert_motion.getVel() == 0)
            removeBox(shapes.at(i).getIndex());
    }
}

void Trail::createBox(float x, float y, float time, Theme theme, PhysicsEngine &physics) {
    // Center position
    x -= _boxes_length/2;
    y -= _boxes_length/2;

    /* Increase size of array and check for exceptions */
    try {
        shapes.resize(shapes.size() + 1, Box(x, y, _boxes_length, (int)shapes.size(), time, theme));
    }
    catch (std::exception &e) {
        LOGE("Error occured while creating box: %s", e.what());
    }

    LOGI("create_box() ->created %i", (int)shapes.size());

    /* Setup new box */
    Box *box = &shapes[(int)shapes.size() - 1];

    // Create the fountain effect
    physics.generateInitVelocity(*box);

    // Just for testing
    // box->hori_motion.setAccel(-1.3f);

    // Allow gravity to affect the box
    box->vert_motion.setAccel(physics.getGravity());
    // box->vert_motion.setAccel(-0.1f);
    
    // box->rot_angle = box->hori_motion.getVel() + box->vert_motion.getVel();
}

void Trail::removeBox(int index) {
    // Shift box positions
    if (index < (int)shapes.size() - 1) {
        for (int i = index; i < (int)shapes.size(); i++) {
            shapes[i] = shapes[i + 1];
            shapes[i].setIndex(i);
        }
    }

    // Decrease size of array & realloc with less mem
    shapes.resize((int)shapes.size() - 1);
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
