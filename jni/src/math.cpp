#include <cmath>
#include "math.h"

#define PI 3.14159265358979323846264

Point2D Math::rotatePt(Point2D pt, float angle) {
    // Rotate
    float rad_angle = angle*PI/180.0f;
    return Point2D(pt.getX()*cos(rad_angle) + pt.getY()*sin(rad_angle),
                  -pt.getX()*sin(rad_angle) + pt.getY()*cos(rad_angle));
}
Point2D Math::rotateObj(Object *obj) {
    // Translate to center
    Point2D pt (obj->getX() + (obj->getWidth()/2),
                obj->getY() + (obj->getHeight()/2));

    // Rotate
    Point2D rotated(rotatePt(pt, obj->getRotAngle()));

    // Translate back to origin
    rotated.setX(rotated.getX() - (obj->getWidth()/2));
    rotated.setY(rotated.getY() - (obj->getHeight()/2));
    return rotated;
}


float Math::getHypotenuse(float a, float b) {
    // Pythagorean Theorem: a^2 + b^2 = c^2
    return sqrt(pow(a, 2.0f) + pow(b, 2.0f));
}

float Math::normalizeAngle(float angle, float min, float max) {
    float newAngle = angle;
    while (newAngle < -min) newAngle += 360;
    while (newAngle >= max) newAngle -= 360;
    return newAngle;
}
