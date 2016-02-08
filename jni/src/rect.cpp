#include "rect.h"
#include "log.h"

Rect::Rect(float x, float y, float width, float height) {
    _x      = x;
    _y      = y;
    _width  = width;
    _height = height;
}
Rect::Rect(Point2D pt1, Point2D pt2) {
    _x = pt1.getX();
    _y = pt1.getY();
    _width = pt2.getX() - pt1.getX();
    _height = pt2.getY() - pt1.getY();
}

Rect::~Rect() { }

float Rect::getX() const {
    return _x;
}
void Rect::setX(float x) {
    _x = x;
}
float Rect::getY() const {
    return _y;
}
void Rect::setY(float y) {
    _y = y;
}
float Rect::getWidth() const {
    return _width;
}
float Rect::getHeight() const {
    return _height;
}
void Rect::setLength(float l) {
    _width = l;
    _height = l;
}
float Rect::getCentreX() const {
    return getX() + (getWidth()/2);
}

float Rect::getCentreY() const {
    return getY() + (getHeight()/2);
}
Point2D Rect::getCentre() const {
    return Point2D(getCentreX(), getCentreY());
}
