#include "trail.h"
#include "log.h"
#include "box.h"
#include "game.h"
#include "math.h"

#define FADE_DEC 0.02f
#define SHRINK_DEC 1.6f
#define OUT_ShapesCount true

Trail::Trail(int obj_length, ObjRenderer* rend) {
    _boxes_length = obj_length;
    _rend = rend;
}

Trail::~Trail() {
    for(int i=0; i<(int)shapes.size(); i++) {
        // Clear boxes array
        std::vector<Shape>().swap(shapes);
    }
}

void Trail::fade(Shape &shape) {
    // fade the box when it has stopped moving vertically
    if (shape.getAlpha() >= FADE_DEC)
        shape.setAlpha(shape.getAlpha() - FADE_DEC);
    else if (shape.getAlpha() > 0.0f)
        shape.setAlpha(0.0f);
}

void Trail::shrink(Shape &shape) {
    if (shape.getWidth() > 0)
        shape.setLength(shape.getWidth() - (SHRINK_DEC*Game::getTimeSpeed())); 
}

void Trail::render(PhysicsEngine &physics) {
    // Draw every shape in shapes vector
    for (int i=0; i<(int)shapes.size(); i++) {
        // Effects
        shrink(shapes.at(i));
        if (shapes.at(i).vert_motion.getVel() == 0)
            fade(shapes.at(i));

        // Send shape to renderer
        shapes.at(i).draw(_rend);

        // Update physics attributes only if box is moving
        if (shapes.at(i).vert_motion.getVel() != 0.0f || shapes.at(i).hori_motion.getVel() != 0.0f)
            physics.updatePhysics(shapes.at(i), Game::getElapsedTime(), Game::getScreenWidth(), Game::getScreenHeight());


        // Remove shape if no longer needed
        if ((shapes.at(i).getAlpha() <= 0 && (int)shapes.size() > 0 && shapes.at(i).vert_motion.getVel() == 0)
                || shapes.at(i).getWidth() == 0
                || shapes.at(i).getWidth() < SHRINK_DEC*Game::getTimeSpeed()) {
            removeBox(shapes.at(i).getIndex());
        }

    }

    _rend->disableAttributes();
}

void Trail::buildTrail(float x, float y, float rot_angle, Theme theme, PhysicsEngine &physics) {
    // Center position
    x -= _boxes_length/2;
    y -= _boxes_length/2;

    /* Setup new box */
    Box box(x, y, (int)shapes.size(), rot_angle, _boxes_length, theme);

    // Apply initial velocity using player rotation
    physics.generateInitVelocity(box, rot_angle);

    /* Increase size of array and check for exceptions */
    try {
        shapes.push_back(box);
    }
    catch (std::exception &e) {
        LOGE("Error occured while creating box: %s", e.what());
    }

    if (OUT_ShapesCount)
        LOGI("%i Shapes (+)", (int)shapes.size());
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
    try {
        shapes.pop_back();
    }
    catch (std::exception &e) {
        LOGE("Error occured while removing box: %s", e.what());
    }

    if (OUT_ShapesCount)
        LOGI("%i Shapes (-)", (int)shapes.size());
}
