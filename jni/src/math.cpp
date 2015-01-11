#include <cmath>
#include "math.h"

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
