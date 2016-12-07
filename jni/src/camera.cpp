#include "camera.h"
#include "collision.h"
#include "log.h"
#include "game.h"

Camera::Camera(const Object *anchor) : _anchor(anchor) { 
    _rot_offset_angle = 0.0f;
    _scale = 1.0f;
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

void Camera::setScaleFromDisp(float d) {
    float max = 2500.0f;
    float min_s = 0.60f;
    float max_s = 1.70f;
    float scale = 0.0f;

    scale = max_s*(1-(d/max));

    if (scale >= min_s && scale <= max_s)
        _scale = scale;
}

Point2D Camera::getPos() {
    return Point2D(_anchor->getCentre().getX() - _pos_offset.getX(), 
                   _anchor->getCentre().getY() - _pos_offset.getY());
}

float Camera::getRotAngle() const {
    return 0.0f;//-_anchor->getRotAngle() + _rot_offset_angle;
}

float Camera::getScale() const {
    return _scale;
}

bool Camera::isObjectInCam(Object *obj) {
    Point2D pos = _anchor->getCentre();
    float borderX = Game::getScreenWidth()/getScale();
    float borderY = Game::getScreenHeight()/getScale();

    return Collision::isBoundingBox(Rect(pos.getX() - borderX/2, pos.getY() -borderY/2, borderX, borderY), *obj);
}

bool Camera::isPtInCam(Point2D pt, float offset) {
    Point2D pos = _anchor->getCentre();
    float borderX = (Game::getScreenWidth()+offset)/getScale();
    float borderY = (Game::getScreenHeight()+offset)/getScale();

    return Collision::isPtInRect(pt, Rect(pos.getX() - borderX/2, pos.getY() -borderY/2, borderX, borderY));
}
