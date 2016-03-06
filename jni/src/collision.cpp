#include <cmath>
#include "collision.h"
#include "config.h"
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
    float ex,ey,fx,fy;

    ex=B.getX()-A.getX(); ey=B.getY()-A.getY();
    fx=D.getX()-A.getX(); fy=D.getY()-A.getY();

    if ((pt.getX()-A.getX())*ex+(pt.getY()-A.getY())*ey<0.0) return false;
    if ((pt.getX()-B.getX())*ex+(pt.getY()-B.getY())*ey>0.0) return false;
    if ((pt.getX()-A.getX())*fx+(pt.getY()-A.getY())*fy<0.0) return false;
    if ((pt.getX()-D.getX())*fx+(pt.getY()-D.getY())*fy>0.0) return false;

    return true;
}

bool Collision::isCircleIntersPolygon(Rect circle, float rot_angle, std::vector<float> vert) {
    for (int i=0; i<(int)vert.size(); i+=2) {
        // Vertices A,B represent both vertex ends of the current edge
        Point2D A = Point2D(vert.at(i), vert.at(i+1));
        Point2D B = (i+2 < (int)vert.size()) ? 
                    Point2D(vert.at(i+2), vert.at(i+3)) :
                    Point2D(vert.at(0), vert.at(1));

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
    }

    // We reach here only if the circle's centre is intersecting with all edges of the polygon
    return true;
}

bool Collision::isCircleInRegion(Rect circle, float region, CollisionData *data, Point2D cur_unit_vec) {
    vector<float> vert = data->planet->getVertices();
    float rot_angle = data->planet->getRotAngle();
    int i = region*2;

    // Vertices A,B represent both vertex ends of the current region edge
    Point2D A = Point2D(vert.at(i), vert.at(i+1));
    Point2D B = (i+2 < (int)vert.size()) ? 
                Point2D(vert.at(i+2), vert.at(i+3)) :
                Point2D(vert.at(0), vert.at(1));

    // Calc unit vector with axis parallel to the normal vector (normal is perpendicular to AB)
    Point2D unit_vec = Math::getUnitVector(Math::getNormal(A, B));

    // Calc base of the circle
    Point2D circle_centre = Math::rotatePt(circle.getCentre(), rot_angle);  // needs to be rotated to work with rotated polygons
    Point2D base;
    if (cur_unit_vec == Point2D(0,0))
        base = circle_centre - unit_vec*(circle.getHeight()/2);  // height is only needed for the offset here as the player always automatically rotates towards the planet so it is parrallel to the normal
    else
        base = circle_centre - cur_unit_vec*(circle.getHeight()/2);  // height is only needed for the offset here as the player always automatically rotates towards the planet so it is parrallel to the normal

    // Calc if this edge is the collided region of the polygon
    if (isPtInRotatedRect(base, A, B, A + (unit_vec*GOBJ_REGION_HEIGHT))) {
        data->region = region;
        data->unit_vec = Math::rotatePt(Point2D(-unit_vec.getY(), unit_vec.getX()), -rot_angle);  // rotate to account for planets rotation
        data->pt = Math::rotatePt(base, -rot_angle);

        // Calc offset from region edge
        float off;
        for (off=0.0f; !isPtInRotatedRect(base, A, B, A + (unit_vec*GOBJ_REGION_HEIGHT)); off+=0.25f)
            base = circle_centre - (unit_vec*((circle.getHeight()/2) - off));

        data->offset = off;
        return true;
    }
    return false;
}

void Collision::genCollisionData(Rect circle, CollisionData *data, Dir facing, int cur_region, Point2D cur_unit_vec) {
    int vertex_count = data->planet->getVertices().size();
    bool left = (facing == LEFT);

    // If we know the current region
    if (cur_region > -1) {
        // Check the most likely region first
        if (isCircleInRegion(circle, cur_region, data)) return;

        int max_region = (vertex_count/2)-1;
        if (isCircleInRegion(circle, (cur_region > 0) ? cur_region-1 : max_region, data)) return;
        if (isCircleInRegion(circle, (cur_region < max_region) ? cur_region+1 : 0, data)) return;
        if (isCircleInRegion(circle, (cur_region > 0) ? cur_region-1 : max_region, data, cur_unit_vec)) return;
        if (isCircleInRegion(circle, (cur_region < max_region) ? cur_region+1 : 0, data, cur_unit_vec)) return;
    }
    else {
        // Check all regions
        for (int i=(left) ? 0 : vertex_count-2;  // This crazy for loop is needed
                (left) ? i<vertex_count : i>=0;  // in order to work for either LEFT or RIGHT
                i+=(left) ? 2 : -2) {            // direction that the use user is facing.

            if (isCircleInRegion(circle, i/2, data)) return;
        }
    }
}
