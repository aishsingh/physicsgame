#include "trail.h"
#include "log.h"
#include "box.h"
#include "game.h"
#include "math.h"

#define FADE_DEC 0.1f
#define SHRINK_DEC 0.6f
#define OUT_ShapesCount false

Trail::Trail(int obj_length) {
    _boxes_length = obj_length;
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


void Trail::draw(ObjRenderer *rend, vector<Planet*> *g_objs) {
    // Draw every shape in shapes vector
    for (int i=0; i<(int)shapes.size(); i++) {
        // Effects
        shrink(shapes.at(i));
        if (shapes.at(i).vert_motion.getVel() == 0)
            fade(shapes.at(i));

        // Send shape to renderer
        shapes.at(i).draw(rend);

        // Update physics attributes only if box is moving
        if (shapes.at(i).vert_motion.getVel() != 0.0f || shapes.at(i).hori_motion.getVel() != 0.0f) {
            PhysicsEngine::updatePhysics(&shapes.at(i), g_objs);
        }


        // Remove shape if no longer needed
        if ((shapes.at(i).getAlpha() <= 0 && (int)shapes.size() > 0 && shapes.at(i).vert_motion.getVel() == 0)
                || shapes.at(i).getWidth() == 0
                || shapes.at(i).getWidth() < SHRINK_DEC*Game::getTimeSpeed()) {
            removeBox(shapes.at(i).getIndex());
        }
    }
}

void Trail::buildTrail(float x, float y, float rot_angle, Theme theme) {
    // Center position
    x -= _boxes_length/2;
    y -= _boxes_length/2;

    /* Setup new box */
    Box box(x, y, (int)shapes.size(), rot_angle, _boxes_length, theme);
    _cur_colour = box.getColour();

    // Apply initial velocity using player rotation
    PhysicsEngine::genInitVel(box, rot_angle, 20, 18, 5);

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

void Trail::applyGravity(const vector<Planet*> *g_objs) {
    for (int i=0; i<(int)shapes.size(); i++)
        PhysicsEngine::applyGravityTo(shapes.at(i), g_objs);
}

void Trail::resetTime(float t) {
    for(int i=0; i<(int)shapes.size(); i++) {
        shapes.at(i).vert_motion.setTime(t);
        shapes.at(i).hori_motion.setTime(t);
    }
}


Colour Trail::getCurColour() const {
    return _cur_colour;
}
