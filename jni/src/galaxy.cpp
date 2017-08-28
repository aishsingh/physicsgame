#include "galaxy.h"
#include "config.h"
#include "math.h"
#include "game.h"
#include "log.h"

Galaxy::Galaxy(int star_count, int gobj_count) {
    // Populate the screen
    populateStars(star_count);
    populateGravObjects(gobj_count);

    _checkpoint = Point2D(Math::genRand(-GALAXY_RANGE_X/2, GALAXY_RANGE_X/2), 
                          Math::genRand(-GALAXY_RANGE_Y/2, GALAXY_RANGE_Y/2));
}

Galaxy::~Galaxy() {
    for(int i=0; i<(int)_g_objs.size(); i++)
        vector<GravObject*>().swap(_g_objs);
}

void Galaxy::drawStars(ObjRenderer *rend) {
    for (int i=0; i<(int)_stars.size(); i++) {
        // Only draw inside the camera window
        if (rend->getCam()->isObjectInCam(_stars.at(i))) {
            // Skip drawing small stars when scaled out
            if ((rend->getCam()->getScale() <= 0.75f && (_stars.at(i)->getWidth()/2) <= 7.0f) || 
                (rend->getCam()->getScale() <= 0.45f && (_stars.at(i)->getWidth()/2) <= 10.0f)) 
                continue;

            _stars.at(i)->draw(rend);
        }
    }

    // Show checkpoint line
    vector<float> vertices = Shape::genCircleVertices(_checkpoint, 50, 0.0f, 10);
    rend->render(vertices,
            Colour::getColour(BLUE),
            0.0f,
            GL_TRIANGLE_FAN);
}

void Galaxy::populateStars(int count) {
    for (int i=0; i<=count; i++) {
        try {
            _stars.push_back(new Star(Math::genRand(-GALAXY_RANGE_X/2, GALAXY_RANGE_X/2), 
                                      Math::genRand(-GALAXY_RANGE_Y/2, GALAXY_RANGE_Y/2), 
                                      Math::genRand(0.0f, 360.0f), 
                                      Math::genRand(5.0f, 25.0f)));
        }
        catch (std::exception &e) {
            LOGE("Error creating stars: %s", e.what());
        }
    }
}

void Galaxy::populateGravObjects(int count) {
    for (int i=0; i<=count; i++) {
        try { // Create planets
            _g_objs.push_back(new Planet(Math::genRand(-GALAXY_RANGE_X/2, GALAXY_RANGE_X/2), 
                                         Math::genRand(-GALAXY_RANGE_Y/2, GALAXY_RANGE_Y/2), 
                                         Math::genRand(150, 600)));
        }
        catch (std::exception &e) {
            LOGE("Error creating planets: %s", e.what());
        }
    }

}

const vector<GravObject*>* Galaxy::getGravObjs() const {
    return &_g_objs;
}

Point2D Galaxy::getCheckpoint() const {
    return _checkpoint;
}
