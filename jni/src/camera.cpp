#include "camera.h"

Camera::Camera(const Object *anchor) : _anchor(anchor) { 
    _rot_offset_angle = 0.0f;
}

void Camera::setAnchorObj(const Object *obj) {
    _anchor = obj;
}

void Camera::setPosOffset(Point2D offset) {
    _pos_offset = offset;
}

void Camera::setRotAngleOffset(float offset) {
    _rot_offset_angle = offset;
}

Point2D Camera::getPos() {
    return Point2D(_anchor->getCentre().getX() - _pos_offset.getX(), 
                   _anchor->getCentre().getY() - _pos_offset.getY());
}

float Camera::getRotAngle() const {
    return -_anchor->getRotAngle() + _rot_offset_angle;
}
