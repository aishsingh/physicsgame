#include "object.h"
#include "log.h"

Object::Object(float x, float y, float width, float height) {
    _x      = x;
    _y      = y;
    _width  = width;
    _height = height;
    // rot_angle = 0.0f;
}
Object::~Object() {
    _x      = 0;
    _y      = 0;
    _width  = 0;
    _height = 0;
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
// int Object::getIndex() {
//     return _index;
// }
// void Object::setIndex(const int index) {
//     _index = index;
// }
float Object::getWidth() const {
    return _width;
}
float Object::getHeight() const {
    return _height;
}

