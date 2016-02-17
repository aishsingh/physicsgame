#include <cmath>
#include "collision.h"
#include "physics.h"
#include "math.h"

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

bool Collision::isPtInRotatedRect(Point2D pt, Point2D A, Point2D B, Point2D D) {
    // Source: http://www.gamedev.net/topic/142526-checking-if-a-point-is-inside-a-rotated-rectangle
    // Point2D diff_hori = B - A;
    // Point2D diff_vert = D - A;
    //
    // if (((pt.getX()-A.getX())*diff_hori.getX())+((pt.getY()-A.getY())*diff_hori.getY()) < 0.0f) return false;
    // if (((pt.getX()-B.getX())*diff_hori.getX())+((pt.getY()-B.getY())*diff_hori.getY()) > 0.0f) return false;
    // if (((pt.getX()-A.getX())*diff_vert.getX())+((pt.getY()-A.getY())*diff_vert.getY()) < 0.0f) return false;
    // if (((pt.getX()-D.getX())*diff_vert.getX())+((pt.getY()-D.getY())*diff_vert.getY()) > 0.0f) return false;
    //
    // return true;

    float ex,ey,fx,fy;

    ex=B.getX()-A.getX(); ey=B.getY()-A.getY();
    fx=D.getX()-A.getX(); fy=D.getY()-A.getY();

    if ((pt.getX()-A.getX())*ex+(pt.getY()-A.getY())*ey<0.0) return false;
    if ((pt.getX()-B.getX())*ex+(pt.getY()-B.getY())*ey>0.0) return false;
    if ((pt.getX()-A.getX())*fx+(pt.getY()-A.getY())*fy<0.0) return false;
    if ((pt.getX()-D.getX())*fx+(pt.getY()-D.getY())*fy>0.0) return false;

    return true;
}

bool Collision::isCircleIntersPolygon(Rect circle, float rot_angle, std::vector<float> vertices, CollisionData *data) {
    /* If we dont care about the collided region or uv, simplify the algorithm to speed things up.
       This is used for non-complex objects such as the boxes in a players trail */
    bool simple_mode = (data == NULL);
    bool collided_region_found = false;

    // Keep searching for furthest region or the closest depending on which way the user is facing at the time of collision
    bool keep_searching = (simple_mode) ? false : (data->facing == LEFT);

    for (int i=0; i<(int)vertices.size(); i+=2) {
        // Vertices A,B represent both vertex ends of the current edge
        Point2D A = Point2D(vertices.at(i), vertices.at(i+1));
        Point2D B = (i+2 < (int)vertices.size()) ? 
                    Point2D(vertices.at(i+2), vertices.at(i+3)) :
                    Point2D(vertices.at(0), vertices.at(1));

        // Calc unit vector with axis parallel to the normal vector (normal is perpendicular to AB)
        Point2D unit_vec = Math::getUnitVector(Math::getNormal(A, B));

        // Calc mid point of the current edge
        Point2D mid = A + ((B - A)/2);

        // Calc base of the circle
        Point2D circle_centre = Math::rotatePt(circle.getCentre(), rot_angle);  // needs to be rotated to work with rotated polygons
        Point2D base = circle_centre - (unit_vec*(circle.getHeight()/2));  // height is only needed for the offset here as the player always automatically rotates towards the planet so it is parrallel to the normal

        // Calc difference between current poly edge and the circle base
        Point2D diff = mid - base;
        float d = Math::dot(diff, unit_vec);

        // See if the circle is not colliding with the edge
        // allowing for a faster fail outcome from the collision algorithm
        if (d*unit_vec.getX() > d || d*unit_vec.getY() > d)
            return false;

        if (!simple_mode && !collided_region_found) {
            // Calc if this edge is the collided region of the polygon
            Point2D pt = circle_centre - (unit_vec*-(circle.getHeight()/3));
            if (isPtInRotatedRect(pt, A, B, A + (unit_vec*110))) {
                data->region = i/2;
                data->unit_vec = unit_vec;

                if (!keep_searching)
                    collided_region_found = true;

                float off;
                for (off=0.0f; !isPtInRotatedRect(base, A, B, A + (unit_vec*110)); off+=0.5f) {
                    base = circle_centre - (unit_vec*((circle.getHeight()/2) - off));
                }

                data->offset = off;
            }
        }
    }

    // We reach here only if the circle's centre is intersecting with all edges of the polygon
    return true;
}
