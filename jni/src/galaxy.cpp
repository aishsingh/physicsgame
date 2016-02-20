#include "galaxy.h"
#include "config.h"
#include "math.h"
#include "game.h"
#include "log.h"

Galaxy::Galaxy() {
    // Populate the screen
    populate_stars(GALAXY_STARS);
}

void Galaxy::draw(ObjRenderer *rend) {
    for (int i=0; i<(int)_stars.size(); i++) {
        // Only draw inside the camera window
        if (rend->getCam()->isObjectNearCam(_stars.at(i))) {
            // Skip drawing small stars when scaled out
            if ((rend->getCam()->getScale() <= 0.75f && (_stars.at(i)->getWidth()/2) <= 7.0f) || 
                (rend->getCam()->getScale() <= 0.45f && (_stars.at(i)->getWidth()/2) <= 10.0f)) 
                continue;

            _stars.at(i)->draw(rend);
        }
    }
}

void Galaxy::populate_stars(int count) {
    for (int i=0; i<=count; i++) {
        _stars.push_back(new Star(Math::genRand(-GALAXY_RANGE_X, GALAXY_RANGE_X), 
                                  Math::genRand(-GALAXY_RANGE_Y, GALAXY_RANGE_Y), 
                                  Math::genRand(0.0f, 360.0f), 
                                  Math::genRand(5.0f, 25.0f)));
    }
}
