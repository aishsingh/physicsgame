#include "object.h"
#include "log.h"

Object::Object(float x, float y, float width, float height) {
    _x      = x;
    _y      = y;
    _width  = width;
    _height = height;
    LOGI("x:%.2f, y:%.2f, w:%.2f, h:%.2f", _x, _y, _width, _height);
}
Object::~Object() {
    _x      = 0;
    _y      = 0;
    _width  = 0;
    _height = 0;
}

float Object::getX() {
    return _x;
}
void Object::setX(const float x) {
    _x = x;
}
float Object::getY() {
    return _y;
}
void Object::setY(const float y) {
    _y = y;
}
int Object::getIndex() {
    return _index;
}
void Object::setIndex(const int index) {
    _index = index;
}
float Object::getWidth() {
    return _width;
}
float Object::getHeight() {
    return _height;
}

