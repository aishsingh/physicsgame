#include <stdlib.h>    // Needed for rand()
#include "shapes.h"
#include "common.h"
#include "motion.h"
#include "log.h"

Box::Box(const float x, const float y, const float diameter, const int &index, const float &time, const Theme &theme) : Object(x,y,diameter,diameter) {
    vert_motion.setTime(time);
    hori_motion.setTime(time);
    setIndex(index);
    setColour(theme);
}

Colour Box::getColour() {
    return _colour;
}
void Box::setColour(const Theme &theme) {
    switch(theme) {
        case GRAY:
        _colour.r = (rand() % 256) / 256.0f;
        _colour.g = _colour.r;
        _colour.b = _colour.r;
        break;

        case RED:
        _colour.r = (rand() % 256) / 256.0f;
        _colour.g = (rand() % 100) / 256.0f;
        _colour.b = _colour.g;
        break;

        case PURPLE:
        _colour.g = (rand() % 50)  / 256.0f;
        _colour.b = (rand() % 256) / 256.0f;
        _colour.r = _colour.b/2;
        break;

        case BLUE:
        _colour.b = (rand() % 255)   / 256.0f;
        _colour.r = (_colour.b/6)    / 256.0f;
        _colour.g = (_colour.b/1.35) / 256.0f;
        break;

        case RAINBOW:
        _colour.r = (rand() % 256) / 256.0f;
        _colour.g = (rand() % 256) / 256.0f;
        _colour.b = (rand() % 256) / 256.0f;
        break;
    }

    // Reset Alpha value
    _colour.a = 1.0f;
}
float Box::getAlpha() {
    return _colour.a;
}
void Box::setAlpha(const float alpha) {
    _colour.a = alpha;
}
