#include <cmath>
#include "collision.h"
#include "physics.h"

bool Collision::isBoundingBox(Rect box1, Rect box2) {
    // TODO
    return false;
}

bool Collision::isPtInRect(Point2D pt, Rect rect) {
    return (pt.getX() >= rect.getX() && pt.getX() <= rect.getX() + rect.getWidth() && 
            pt.getY() >= rect.getY() && pt.getY() <= rect.getY() + rect.getWidth())
           ? true : false;
}

bool Collision::isPtInRect(int x, int y, Rect rect) {
    return isPtInRect(Point2D(x, y), rect);
}

bool Collision::isCircleIntersCircle(Rect circ1, Rect circ2) {
    float radius_total = circ1.getWidth()/2 + circ2.getWidth()/2;

    // Pythagorean Theorem: a^2 + b^2 = c^2
    float a = fabs(circ1.getCentreX() - circ2.getCentreX());
    float b = fabs(circ1.getCentreY() - circ2.getCentreY());
    float c = sqrt(pow(a, 2.0f) + pow(b, 2.0f));
    
    return (c < radius_total) ? true : false;
}

