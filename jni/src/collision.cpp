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

bool Collision::isCircleIntersPolygon(Rect circle, float rot_angle, std::vector<float> vertices, int *collided_region, Point2D *collided_unit_vector) {
    /* If we dont care about the collided region or uv simplify the algorithm to speed things up.
       This is used for non-complex objects such as the boxes in a players trail */
    bool simple_mode = (collided_region == NULL || collided_unit_vector == NULL);

    for (int i=0; i<(int)vertices.size(); i+=2) {
        // Vertices A,B represent both vertex ends of the current edge
        Point2D A = Point2D(vertices.at(i), vertices.at(i+1));
        Point2D B = (i+2 < (int)vertices.size()) ? 
                Point2D(vertices.at(i+2), vertices.at(i+3)) :
                Point2D(vertices.at(0), vertices.at(1));

        // Calc normal vector (perpendicular to AB)
        Point2D N = Math::getRightNormal(A, B);

        // Calc unit vector with axis parallel to the normal vector
        Point2D unit_vec = Math::getUnitVector(N);

        // Calc mid point of the current edge
        Point2D half = Point2D((B.getX() - A.getX())/2, (B.getY() - A.getY())/2);
        Point2D mid = Point2D(A.getX() + half.getX(), A.getY() + half.getY());

        // Calc base of the circle
        Point2D circle_centre = Math::rotatePt(circle.getCentre(), rot_angle);  // needs to be rotated to work with rotated polygons
        float offset = circle.getHeight()/2;  // offset needed to go from the centre to the base
        Point2D base = Point2D(circle_centre.getX() - (offset*unit_vec.getX()),
                               circle_centre.getY() - (offset*unit_vec.getY()));

        // Calc difference between current poly edge and the circle base
        Point2D diff = Point2D(mid.getX() - base.getX(), mid.getY() - base.getY());
        float d = Math::dot(diff, unit_vec);

        // See if the circle is not colliding with the edge
        // allowing for a faster fail outcome from the collision algorithm
        if (d*unit_vec.getX() > d || d*unit_vec.getY() > d)
            return false;

        if (!simple_mode) {
            // Calc if this edge is the collided region of the polygon
            // Source: http://math.stackexchange.com/a/312733

            Point2D vrA = A;//(i+2 < (int)vertices.size()) ? A : B;
            Point2D vrB = B;//(i+2 < (int)vertices.size()) ? B : A;
            Point2D vrC = Point2D(B.getX()+110*unit_vec.getX(), B.getY()+110*unit_vec.getY());
            Point2D vrD = Point2D(A.getX()+110*unit_vec.getX(), A.getY()+110*unit_vec.getY());
            Point2D vrQ = Point2D((vrA.getX() + vrC.getX())/2, (vrA.getY() + vrC.getY())/2);           // center point
            float a = Math::distance(vrA, vrC);     // half-width (in the x-direction)
            float b = Math::distance(vrB, vrD);     // half-width (in the x-direction)
            Point2D vrU = Math::getUnitVector(Math::getRightNormal(vrC, vrA));//Point2D((vrC.getX() - vrA.getX())/(2*a), (vrC.getY() - vrA.getY())/(2*a));         // unit vector in x-direction
            Point2D vrV = Math::getUnitVector(Math::getRightNormal(vrD, vrB));//Point2D((vrD.getX() - vrB.getX())/(2*b), (vrD.getY() - vrB.getY())/(2*b));         // unit vector in y-direction
            Point2D vrW = Point2D(circle.getCentreX() - vrQ.getX(), circle.getCentreY() - vrQ.getY());
            float xabs = fabs(Math::dot(vrW, vrU));    // here W*U is the dot product of W and U
            float yabs = fabs(Math::dot(vrW, vrV));    // here W*V is the dot product of W and V

            if (xabs/a + yabs/b <= 1) {
                // This is the collided region
                *collided_region = i/2;
                *collided_unit_vector = unit_vec;
                // *collided_unit_vector = (i+2 < (int)vertices.size()) ? unit_vec : Point2D(unit_vec.getX(), -unit_vec.getY());
            }
        }
    }

    // We reach here only if the circle's centre is intersecting with all edges of the polygon
    return true;
}
