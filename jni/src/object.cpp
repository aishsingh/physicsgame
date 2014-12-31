#include "object.h"
#include "log.h"

Object::Object(float x, float y, float width, float height) {
    _x      = x;
    _y      = y;
    _width  = width;
    _height = height;
    _rot_angle = 0.0f;
}
Object::~Object() {
    _x      = 0;
    _y      = 0;
    _width  = 0;
    _height = 0;
    _rot_angle = 0.0f;
}

float Object::getX() const {
    return _x;
}
void Object::setX(float x) {
    _x = x;
}
float Object::getY() const {
    return _y;
}
void Object::setY(float y) {
    _y = y;
}
float Object::getWidth() const {
    return _width;
}
float Object::getHeight() const {
    return _height;
}
void Object::setLength(float l) {
    _width = l;
    _height = l;
}
float Object::getRotAngle() const {
    return _rot_angle;
}
void Object::setRotAngle(float angle) {
    if (angle >= 360)
        angle = 0.0f;

    _rot_angle = angle;
}

float Object::getCentreX() const {
    return getX() + (getWidth()/2);
}

float Object::getCentreY() const {
    return getY() + (getHeight()/2);
}
