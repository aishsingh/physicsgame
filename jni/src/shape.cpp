#include <stdlib.h>    // Needed for rand()
#include "shape.h"

Shape::Shape(float x, float y, float width, float height, float &time, Theme &theme) : Object(x,y,width,height) {
    vert_motion.setTime(time);
    hori_motion.setTime(time);

    rot_angle = 0.0f;

    useTheme(theme);
}

Colour Shape::getColour() const {
    return _colour;
}
void Shape::useTheme(Theme &theme) {
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
float Shape::getAlpha() const {
    return _colour.a;
}
void Shape::setAlpha(float alpha) {
    _colour.a = alpha;
}
