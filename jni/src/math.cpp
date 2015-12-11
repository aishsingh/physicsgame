#include <cmath>
#include "math.h"
#include "log.h"

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

float Math::getRectDispFromRect(Rect rect1, Rect rect2) {
    // Pythagorean Theorem: a^2 + b^2 = c^2
    float a = fabs(rect1.getCentreX() - rect2.getCentreX());
    float b = fabs(rect1.getCentreY() - rect2.getCentreY());
    float c = fabs(Math::getHypotenuse(a, b));
    
    return c;
}

float Math::getValueFromRatioBetweenValues(float a, float b, float ratio) {
    // Validate ratio param
    if (ratio < 0 || ratio > 1)
        LOGE("Math Error -> ratio param is not valid");

    return  a / pow (a/b, ratio);
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

float Math::genRand(float min, float max) {
    return min + (float)(rand()) / (float)(RAND_MAX/(max-min));
}

vector<float> Math::genRandData(int count, float min, float max) {
    vector<float> data;
    for (int i=0; i<count; i++) {
        data.push_back(genRand(min, max));
    }

    return data;
}

vector<float> Math::offsetDataByRand(vector<float> data, float min, float max) {
    for (int i=0; i < data.size(); i++) {
        float offset = min + (float)(rand()) / (float)(RAND_MAX/(max-min));
        data.at(i) += offset;
    }
    
    return data;
}

vector<float> Math::offsetDataByData(vector<float> data, vector<float> offset) {
    for (int i=0; i < data.size(); i++) {
        data.at(i) += offset.at(i);
    }
    
    return data;
}


Point2D Math::normalize(Point2D pt) {
    float length = sqrt(pt.getX()*pt.getX() + pt.getY()*pt.getY());
    return Point2D(pt.getX()/length,
                   pt.getY()/length);
}

float Math::dot(Point2D vec1, Point2D vec2) {
    return (vec1.getX()*vec2.getX()) + (vec1.getY()*vec2.getY());
}

void Math::project(Point2D axis, vector<float> vertices, float *min, float *max) {
    // To project a point on an axis use the dot product
    float dp = dot(axis, Point2D(vertices.at(0), vertices.at(1)));
    *min = dp;
    *max = dp;

    for (int i = 0; i < vertices.size()/2; i+=2) {
        dp = dot(axis, Point2D(vertices.at(i), vertices.at(i+1)));
        if (dp < *min) *min = dp;
        else if (dp > *max) *max = dp;
    }

}
