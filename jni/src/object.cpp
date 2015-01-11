#include "object.h"
#include "math.h"

Object::Object(float x, float y, float width, float height) :  Rect(x,y,width,height), _rot_angle(0.0f) { }
Object::~Object() {
    _rot_angle = 0.0f;
}
float Object::getRotAngle() const {
    return _rot_angle;
}
void Object::setRotAngle(float angle) {
    _rot_angle = Math::normalizeAngle(angle, 0, 360);
}

Point2D Object::getPos() const {
    return Point2D(getX(), getY());
}
