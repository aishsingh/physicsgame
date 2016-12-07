#include "galaxy.h"
#include "config.h"
#include "math.h"
#include "game.h"
#include "log.h"

Galaxy::Galaxy() {
    // Populate the screen
    populate_stars(GALAXY_STARS);
    _checkpoint = Point2D(Math::genRand(-GALAXY_RANGE_X, GALAXY_RANGE_X), 
                          Math::genRand(-GALAXY_RANGE_Y, GALAXY_RANGE_Y));
}

void Galaxy::draw(ObjRenderer *rend) {
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

void Galaxy::populate_stars(int count) {
    for (int i=0; i<=count; i++) {
        _stars.push_back(new Star(Math::genRand(-GALAXY_RANGE_X, GALAXY_RANGE_X), 
                                  Math::genRand(-GALAXY_RANGE_Y, GALAXY_RANGE_Y), 
                                  Math::genRand(0.0f, 360.0f), 
                                  Math::genRand(5.0f, 25.0f)));
    }
}

Point2D Galaxy::getCheckpoint() const {
    return _checkpoint;
}
