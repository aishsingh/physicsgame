#include <cmath>
#include "collision.h"
#include "config.h"
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

bool Collision::isPtInTriangle(Point2D pt, Point2D A, Point2D B, Point2D C) {
    // Source: http://stackoverflow.com/a/20861130
    float s = A.getY() * C.getX() - A.getX() * C.getY() + (C.getY() - A.getY()) * pt.getX() + (A.getX() - C.getX()) * pt.getY();
    float t = A.getX() * B.getY() - A.getY() * B.getX() + (A.getY() - B.getY()) * pt.getX() + (B.getX() - A.getX()) * pt.getY();

    if ((s < 0) != (t < 0))
        return false;

    float d = -B.getY() * C.getX() + A.getY() * (C.getX() - B.getX()) + A.getX() * (B.getY() - C.getY()) + B.getX() * C.getY();
    if (d < 0.0)
    {
        s = -s;
        t = -t;
        d = -d;
    }
    return s > 0 && t > 0 && (s + t) <= d;
}

bool Collision::isCircleIntersPolygon(Rect circle, float rot_angle, std::vector<float> vert) {
    for (int i=0; i<(int)vert.size()-1; i+=2) {
        // Vertices A,B represent both vertex ends of the current edge
        Point2D A = Point2D(vert.at(i), vert.at(i+1));
        Point2D B = (i+2 < (int)vert.size()-1) ? 
                        Point2D(vert.at(i+2), vert.at(i+3)) :
                        Point2D(vert.at(0), vert.at(1));

        // Calc unit vector with axis parallel to the normal vector (normal is perpendicular to AB)
        Point2D unit_vec = Math::getUnitVector(Math::getNormal(A, B));

        // Calc mid point of the current edge
        Point2D mid = B - ((B - A)/2);

        // Calc base of the circle
        Point2D circle_centre = Math::rotatePt(circle.getCentre(), rot_angle);  // needs to be rotated to work with rotated polygons
        Point2D base = circle_centre - (unit_vec*(circle.getHeight())/2);  // height is only needed for the offset here as the player always automatically rotates towards the planet so it is parrallel to the normal

        // Calc difference between current poly edge and the circle base
        Point2D diff = mid - base;
        float d = Math::dot(diff, unit_vec);

        // See if the circle is not colliding with the edge
        // allowing for a faster fail outcome from the collision algorithm
        if (d*unit_vec.getX() > d && d*unit_vec.getY() > d)
            return false;
    }

    // We reach here only if the circle's centre is intersecting with all edges of the polygon
    return true;
}

bool Collision::isCircleInRegion(Rect circle, unsigned region, CollisionData *data) {
    vector<float> vert = data->planet->getVertices();
    float rot_angle = data->planet->getRotAngle();

    Point2D uv_a = data->planet->getUnitVectorAt(region/2);

    // Calc base of the circle
    Point2D circle_centre = Math::rotatePt(circle.getCentre(), rot_angle);  // needs to be rotated to work with rotated polygons
    Point2D base = circle_centre - uv_a*(circle.getHeight()/2);  // height is only needed for the offset here as the player always automatically rotates towards the planet so it is parrallel to the normal

    if (region % 2 == 0) {  // Flat edge region
        Point2D A, B;
        if (region < vert.size()-1) {
            A = Point2D(vert.at(region), vert.at(region+1));
            B = (region+2 < vert.size()-1) ? 
                        Point2D(vert.at(region+2), vert.at(region+3)) :
                        Point2D(vert.at(0), vert.at(1));
        }
        else {  // If checking the last region
            A = Point2D(vert.at(region-1), vert.at(region));
            B = Point2D(vert.at(0), vert.at(1));
        }

        // Calc if collided with flat region of the polygon
        if (isPtInRotatedRect(circle_centre, A, B, A + (uv_a*GOBJ_REGION_HEIGHT))) {
            data->region = region;
            data->unit_vec = Math::rotatePt(Point2D(-uv_a.getY(), uv_a.getX()), -rot_angle);  // rotate to account for planets rotation
            data->pt = Math::rotatePt(base, -rot_angle);

            // Calc offset from region edge
            float off;
            for (off=0.0f; !isPtInRotatedRect(base, A, B, A + (uv_a*GOBJ_REGION_HEIGHT)); off+=0.25f)
                base = circle_centre - (uv_a*((circle.getHeight()/2) - off));

            data->offset = off;
            return true;
        }
    }
    else {  // Corner edge region
        Point2D B = (region+1 < vert.size()-1) ? 
                        Point2D(vert.at(region+1), vert.at(region+2)) :
                        Point2D(vert.at(0), vert.at(1));

        Point2D uv_b = (region+1 < vert.size()-1)? 
                        data->planet->getUnitVectorAt((region/2)+1) : 
                        data->planet->getUnitVectorAt(0);

        // Calc if collided with corner edge
        if (isPtInTriangle(circle_centre, B, B + (uv_a*GOBJ_REGION_HEIGHT), B + (uv_b*GOBJ_REGION_HEIGHT))) {
            data->region = region;
            data->unit_vec = Math::rotatePt(Point2D(-uv_a.getY(), uv_a.getX()), -rot_angle);  // rotate to account for planets rotation
            data->pt = Math::rotatePt(base, -rot_angle);

            return true;
        }
    }

    return false;
}

void Collision::genCollisionData(Rect circle, CollisionData *data, Dir facing, int cur_region) {
    int vertex_count = data->planet->getVertices().size();
    bool left = (facing == LEFT);

    // If we know the current region we check the most likely regions first
    if (cur_region > -1) {
        // Check current first
        if (isCircleInRegion(circle, cur_region, data)) return;

        // Check neighbouring regions
        int max_region = vertex_count-1;
        int left_region = (cur_region > 0) ? cur_region-1 : max_region;
        int right_region = (cur_region < max_region) ? cur_region+1 : 0;

        if (isCircleInRegion(circle, left_region, data)) return;
        if (isCircleInRegion(circle, right_region, data)) return;
    }

    // Check all regions
    for (int i=(left) ? 0 : vertex_count-1;    // This crazy for loop is needed
            (left) ? i<vertex_count-1 : i>=0;  // in order to work for either LEFT or RIGHT
            i+=(left) ? 1 : -1) {              // direction that the user is facing.

        if (isCircleInRegion(circle, i, data)) return;
    }
}
