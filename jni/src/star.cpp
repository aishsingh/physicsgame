#include "star.h"
#include "math.h"
#include "shape.h"

Star::Star(float x, float y, float angle, float diameter) : Object(x,y,diameter,diameter,angle) {
    // randonly gen a type
    _type = static_cast<StarType>(Math::genRand(0, 2));

    _scale = Math::genRand(0.0f, 1.0f);
    _scale_speed = Math::genRand(0.002f, 0.02f);
    _growing = false;
}

Star::StarType Star::getType() const {
    return _type;
}

float Star::getScale() const {
    return _scale;
}

void Star::draw(ObjRenderer *rend) {
    rend->render(Shape::genCircleVertices(getCentre(), (getWidth()/2)*_scale, getRotAngle(), 5),
                 Colour(1.0, 1.0, 1.0, 1.0),
                 getRotAngle(),
                 GL_TRIANGLE_FAN);

    // Scale
    if (_type == SHINING)
        growAndShrink();
}

void Star::growAndShrink() {
    // increment/decrement
    if (_growing)
        _scale += _scale_speed;
    else
        _scale -= _scale_speed;

    // Boundary conditions
    if (_scale >= 1 && _growing) {
        _growing = false;
    }
    else if (_scale <= 0 && !_growing) {
        _growing = true;
    }
}

