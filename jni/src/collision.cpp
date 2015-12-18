#include <cmath>
#include <limits>       // std::numeric_limits
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

// bool Collision::isCircleIntersPolygon(Rect circle, Rect poly, std::vector<float> vertices) {
//     float min_dist = std::numeric_limits<int>::max();
//     Point2D trans_axis;
//
//     for (int i=0; i<2; i+=2) {
//         Point2D axis = Point2D(-vertices.at(i+1), vertices.at(i));
//         axis = Math::normalize(axis);
//
//         vector<float> circle_vertices; // block
//         circle_vertices.push_back(circle.getX());
//         circle_vertices.push_back(circle.getY());
//         circle_vertices.push_back(circle.getX()+circle.getWidth());
//         circle_vertices.push_back(circle.getY());
//         circle_vertices.push_back(circle.getX()+circle.getWidth());
//         circle_vertices.push_back(circle.getY()+circle.getHeight());
//         circle_vertices.push_back(circle.getX());
//         circle_vertices.push_back(circle.getY()+circle.getHeight());
//
//         // Find the projection of the polygon on the current axis
//         float minA = 0; float minB = 0; float maxA = 0; float maxB = 0;
//         Math::project(axis, vertices, &minA, &maxA);
//         Math::project(axis, circle_vertices, &minB, &maxB);
//         // minB = circle.getWidth()/2;
//         // maxB = circle.getWidth()/2;
//
//         // Check if the polygon projections are currentlty intersecting
//         float dist;
//         if (minA < minB) {
//             dist = minB - maxA;
//         } else {
//             dist = minA - maxB;
//         }
//         LOGI("dist = %.2f, minA %.2f, maxA %.2f, minB %.2f, maxB %.2f", dist, minA, maxA, minB, maxB);
//         if (dist > 0) 
//             return false;
//
//         if (dist < min_dist) {
//             min_dist = dist;
//             trans_axis = axis;
//
//             Point2D circle_centre = Point2D(circle.getX() + (circle.getWidth()/2),
//                                             circle.getY() + (circle.getHeight()/2));
//             Point2D poly_centre = Point2D(poly.getX() + (poly.getWidth()/2),
//                                             poly.getY() + (poly.getHeight()/2));
//
//             Point2D d = Point2D(poly_centre.getX() - circle_centre.getX(),
//                                 poly_centre.getY() - circle_centre.getY());
//             if (Math::dot(d, trans_axis) < 0)
//                 trans_axis = Point2D(-trans_axis.getX(), -trans_axis.getY());
//         }
//
//         // sat_pts.push_back(1);
//     }
//
//     return true;
// }

bool Collision::isCircleIntersPolygon(Rect circle, std::vector<float> vertices) {
    for (int i=0; i<vertices.size(); i+=2) {
        // Vertices A,B represent both vertex ends of the current edge
        Point2D A = Point2D(vertices.at(i), vertices.at(i+1));
        Point2D B = (i+2 < vertices.size()) ? 
                Point2D(vertices.at(i+2), vertices.at(i+3)) :
                Point2D(vertices.at(0), vertices.at(1));

        // Calc normal vector (perpendicular to AB)
        Point2D AB = Point2D(B.getX() - A.getX(), B.getY() - A.getY()); 
        Point2D N = Point2D(AB.getY(), -AB.getX());

        // Calc unit vector with axis parallel to the normal vector
        float mag = sqrt(pow(N.getX(), 2) + pow(N.getY(), 2));
        Point2D unit_vec = Point2D(N.getX()/mag, N.getY()/mag);

        // Calc mid point of the current edge
        Point2D half = Point2D((B.getX() - A.getX())/2, (B.getY() - A.getY())/2);
        Point2D mid = Point2D(A.getX() + half.getX(), A.getY() + half.getY());

        Point2D base = Point2D(circle.getCentreX() - ((circle.getHeight()/2)*unit_vec.getX()), circle.getCentreY() - ((circle.getHeight()/2)*unit_vec.getY()));

        // Calc difference between current poly edge and the circle centre
        Point2D diff = Point2D(mid.getX() - base.getX(), mid.getY() - base.getY());
        float d = Math::dot(diff, unit_vec);

        // See if the circle is not colliding with the edge
        // allowing for a faster fail outcome from the collision algorithm
        if (d*unit_vec.getX() > d && d*unit_vec.getY() > d)
            return false;
    }

    // We reach here only if the circle's centre is intersecting with all edges of the polygon
    return true;
}
