#include "motion.h"

Motion::Motion() {
        _t = 0.0f;
        _d = 0.0f;
        _v = 0.0f;
        _a = 0.0f;
}
Motion::~Motion() {
        _t = 0.0f;
        _d = 0.0f;
        _v = 0.0f;
        _a = 0.0f;
}

float Motion::getTime() const {
    return _t;
}
void Motion::setTime(float t) {
    _t = t;
}
float Motion::getDisp() const {
    return _d;
}
void Motion::setDisp(float d) {
    _d = d;
}
float Motion::getVel() const {
    return _v;
}
void Motion::setVel(float v) {
    _v = v;
}
float Motion::getAccel() const {
    return _a;
}
void Motion::setAccel(float a) {
    _a = a;
}

