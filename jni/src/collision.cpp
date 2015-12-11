#include <cmath>
#include "collision.h"
#include "physics.h"
#include "math.h"
#include "log.h"

bool Collision::isBoundingBox(Rect box1, Rect box2) {
    return (box1.getX() < box2.getX() + box2.getWidth() &&
            box1.getX() + box1.getWidth() > box2.getX() &&
            box1.getY() < box2.getY() + box2.getHeight() &&
            box1.getHeight() + box1.getY() > box2.getY()) 
           ? true : false;
}

bool Collision::isPtInRect(Point2D pt, Rect rect) {
    return (pt.getX() >= rect.getX() && pt.getX() <= rect.getX() + rect.getWidth() && 
            pt.getY() >= rect.getY() && pt.getY() <= rect.getY() + rect.getHeight())
           ? true : false;
}

bool Collision::isPtInRect(int x, int y, Rect rect) {
    return isPtInRect(Point2D(x, y), rect);
}

bool Collision::isCircleIntersCircle(Rect circ1, Rect circ2) {
    float radius_total = circ1.getWidth()/2 + circ2.getWidth()/2;
    float c = Math::getRectDispFromRect(circ1, circ2);
    return (c < radius_total) ? true : false;
}

bool Collision::isPtInCircle(Point2D pt, Rect circ) {
    float radius_total = circ.getWidth()/2;

    // Pythagorean Theorem: a^2 + b^2 = c^2
    float a = fabs(pt.getX() - circ.getCentreX());
    float b = fabs(pt.getY() - circ.getCentreY());
    float c = Math::getHypotenuse(a, b);
    
    return (c < radius_total) ? true : false;
}

bool Collision::isCircleIntersPolygon(Rect circle, Rect poly, std::vector<float> vertices) {
    for (int i=0; i<vertices.size(); i+=2) {
        Point2D axis = Point2D(vertices.at(i+1), vertices.at(i));
        axis = Math::normalize(axis);

        vector<float> circle_vertices; // block
        circle_vertices.push_back(circle.getX());
        circle_vertices.push_back(circle.getY());
        circle_vertices.push_back(circle.getX()+circle.getWidth());
        circle_vertices.push_back(circle.getY());
        circle_vertices.push_back(circle.getX()+circle.getWidth());
        circle_vertices.push_back(circle.getY()+circle.getHeight());
        circle_vertices.push_back(circle.getX());
        circle_vertices.push_back(circle.getY()+circle.getHeight());

        // Find the projection of the polygon on the current axis
        float minA = 0; float minB = 0; float maxA = 0; float maxB = 0;
        Math::project(axis, vertices, &minA, &maxA);
        Math::project(axis, circle_vertices, &minB, &maxB);

        // Check if the polygon projections are currentlty intersecting
        float dist;
        if (minA < minB) {
            dist = minB - maxA;
        } else {
            dist = minA - maxB;
        }
        LOGI("dist = %.2f", dist);
        if (dist > 0) 
            return false;
    }
    return true;
}
