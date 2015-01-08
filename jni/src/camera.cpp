#include "camera.h"

Camera::Camera(const Object *anchor) : _anchor(anchor), _offset(0, 0) { }

void Camera::setAnchorObj(const Object *obj) {
    _anchor = obj;
}

void Camera::setOffset(Point2D offset) {
    _offset = offset;
}

Point2D Camera::getPos() const {
    return _anchor->getCentre();
}

float Camera::getRotAngle() const {
    return -_anchor->getRotAngle();
}
