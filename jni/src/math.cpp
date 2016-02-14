#include <cmath>
#include "math.h"
#include "log.h"

#define PI 3.14159265358979323846264

float Math::distance(Point2D pt1, Point2D pt2) {
    return sqrtf(powf(pt2.getX() - pt1.getX(), 2) + powf(pt2.getY() - pt1.getY(), 2));
}

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

    return a / pow (a/b, ratio);
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


vector<float> Math::getUnitVectors(vector<float> vertices) {
    vector<float> unit_vectors;
    for (int i=0; i<(int)vertices.size(); i+=2) {
        Point2D A = Point2D(vertices.at(i), vertices.at(i+1));
        Point2D B, uv;

        if (i+2 < (int)vertices.size()) {
            B = Point2D(vertices.at(i+2), vertices.at(i+3));
            uv = getUnitVector(getNormal(A, B));
        }
        else {
            B = Point2D(vertices.at(0), vertices.at(1));
            uv = getUnitVector(getNormal(A, B));
        }


        unit_vectors.push_back(uv.getX());
        unit_vectors.push_back(uv.getY());
    }

    return unit_vectors;
}

vector<float> Math::genRandData(int count, float min, float max) {
    vector<float> data;
    for (int i=0; i<count; i++) {
        data.push_back(genRand(min, max));
    }

    return data;
}

vector<float> Math::offsetDataByRand(vector<float> data, float min, float max) {
    for (int i=0; i < (int)data.size(); i++) {
        float offset = min + (float)(rand()) / (float)(RAND_MAX/(max-min));
        data.at(i) += offset;
    }
    
    return data;
}

vector<float> Math::offsetDataByData(vector<float> data, vector<float> offset) {
    for (int i=0; i < (int)data.size(); i++) {
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

    for (int i = 0; i < (int)vertices.size()/2; i+=2) {
        dp = dot(axis, Point2D(vertices.at(i), vertices.at(i+1)));
        if (dp < *min) *min = dp;
        else if (dp > *max) *max = dp;
    }

}

Point2D Math::getNormal(Point2D A, Point2D B) {
    Point2D AB = Point2D(B.getX() - A.getX(), B.getY() - A.getY()); 

    // Normal vector (perpendicular to the right side of AB)
    return Point2D(AB.getY(), -AB.getX());
}

Point2D Math::getUnitVector(Point2D normal) {
    // Calc unit vector with axis parallel to the normal vector
    float mag = sqrt(pow(normal.getX(), 2) + pow(normal.getY(), 2));
    return Point2D(normal.getX()/mag, normal.getY()/mag);
}

Point2D Math::rotatePtAroundPt(Point2D pt1, Point2D pt2, float angle) {
    float rad = angle*(PI/180);

    /* http://stackoverflow.com/a/12161405/3270542 */
    return Point2D(pt1.getX() + (pt2.getX()-pt1.getX())*cos(rad) - (pt2.getY()-pt1.getY())*sin(rad),
                   pt1.getY() + (pt2.getX()-pt1.getX())*sin(rad) + (pt2.getY()-pt1.getY())*cos(rad));

}

float Math::calcZComponent(float vertices[6]) {
    /* http://stackoverflow.com/a/1881201 */

    float dx1 = vertices[2]-vertices[0];
    float dy1 = vertices[3]-vertices[1];
    float dx2 = vertices[4]-vertices[2];
    float dy2 = vertices[5]-vertices[3];

    return dx1*dy2 - dy1*dx2;
}

vector<float> Math::removeConcaveVertices(vector<float> vertices, vector<float> *offset) {
    int expected_sign = 0; // -1 = neg, 0 = unknown, 1 = pos
    for (int i=0; i<(int)vertices.size(); i+=2) {
        bool backwards = true;

        float *indexes;
        Point2D mid_index;
        if (i+2 >= (int)vertices.size()) {
            indexes = (float[6]){i, i+1,
                                 0, 1,
                                 2, 3};
        } 
        else if (i+4 >= (int)vertices.size()) {
            indexes = (float[6]){i,   i+1,
                                 i+2, i+3,
                                 0,   1};
        }
        else {
            indexes = (float[6]){i,   i+1,
                                 i+2, i+3,
                                 i+4, i+5};
            backwards = false;
        }


        float v[6] = {vertices.at(indexes[0]), vertices.at(indexes[1]),
                      vertices.at(indexes[2]), vertices.at(indexes[3]),
                      vertices.at(indexes[4]), vertices.at(indexes[5])};

        float z_comp = calcZComponent(v);

        if (expected_sign == 0) {
            if (z_comp > 0)
                expected_sign = 1;
            else
                expected_sign = -1;
        }
        else if (z_comp*expected_sign < 0) {
            // concave detected
            LOGI("concave at [(%i), (%i), (%i)]", i/2, (i+2)/2, (i+4)/2);
            LOGI("");
            LOGI("    (%i), offset - (%.2f, %.2f)", i/2,     offset->at(indexes[0]), offset->at(indexes[1]));
            LOGI("  ->(%i), offset - (%.2f, %.2f)", (i+2)/2, offset->at(indexes[2]), offset->at(indexes[3]));
            LOGI("    (%i), offset - (%.2f, %.2f)", (i+4)/2, offset->at(indexes[4]), offset->at(indexes[5]));
            LOGI("");

            Point2D diff = Point2D(offset->at(indexes[4]) - offset->at(indexes[0]),
                                   offset->at(indexes[5]) - offset->at(indexes[1]));
            //
            // offset->at(indexes[2]) = offset->at(indexes[0]) + (diff.getX()/2);
            // offset->at(indexes[3]) = offset->at(indexes[1]) + (diff.getY()/2);
            //
            // LOGI("  <-(%i), offset - (%.2f, %.2f)", (i+2)/2, offset->at(indexes[2]), offset->at(indexes[3]));
            // LOGI("");


            Point2D A = Point2D(vertices.at(indexes[0]), vertices.at(indexes[1]));
            Point2D B = Point2D(vertices.at(indexes[4]), vertices.at(indexes[5]));
            Point2D unit_vec = getUnitVector((backwards) ? getNormal(B, A) : getNormal(A, B));

            // Point2D new_mid = Point2D(vertices.at(indexes[2]) + (offset->at(indexes[2])*unit_vec.getX()), 
            //                           vertices.at(indexes[3]) + (offset->at(indexes[3])*unit_vec.getY()));
            //
            // Point2D diff = Point2D(new_mid.getX() - vertices.at(indexes[2]),
            //                        new_mid.getY() - vertices.at(indexes[3]));


            // offset->at(indexes[2]) = 15*unit_vec.getX();
            // offset->at(indexes[3]) = 15*unit_vec.getY();

            // LOGI("  <-(%i), offset - (%.2f, %.2f)", (i+2)/2, offset->at(indexes[2]), offset->at(indexes[3]));
            // LOGI("");


            // vertices.at(mid_index.getX()) += offset->at(mid_index.getX());
            // vertices.at(mid_index.getY()) += offset->at(mid_index.getY());
        }
    }

    return vertices;
}
