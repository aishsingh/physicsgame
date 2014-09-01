#include "motion.h"

Motion::Motion() {
        _ct = 0.0f;
        _t = 0.0f;
        _d = 0.0f;
        _v = 0.0f;
        _a = 0.0f;
}
Motion::~Motion() {
        _ct = 0.0f;
        _t = 0.0f;
        _d = 0.0f;
        _v = 0.0f;
        _a = 0.0f;
}
float Motion::getChangeTime() {
    return _ct;
}
void Motion::setChangeTime(const float ct) {
    _ct = ct;
}
float Motion::getTime() {
    return _t;
}
void Motion::setTime(const float t) {
    _t = t;
}
float Motion::getDisp() {
    return _d;
}
void Motion::setDisp(const float d) {
    _d = d;
}
float Motion::getVel() {
    return _v;
}
void Motion::setVel(const float v) {
    _v = v;
}
float Motion::getAccel() {
    return _a;
}
void Motion::setAccel(const float a) {
    _a = a;
}

