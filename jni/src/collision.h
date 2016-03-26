/* Description: Collision class used to determine colisions
 * Created On: 2015-01-02 
 */

#ifndef COLLISION_H
#define COLLISION_H

#include <vector>
#include "rect.h"
#include "point2d.h"
#include "grav_object.h"
#include "direction.h"

struct CollisionData {
    GravObject *planet;
    Point2D unit_vec;
    Point2D pt;
    int region;
    float offset;

    CollisionData(GravObject* p =NULL) : planet(p), region(-1), offset(0.0f) { }
};

class Collision {
    public:
        static bool isBoundingBox(Rect box1, Rect box2);
        static bool isPtInRect(Point2D pt, Rect rect);
        static bool isPtInRect(int x, int y, Rect rect);
        static bool isCircleIntersCircle(Rect circ1, Rect circ2);
        static bool isPtInCircle(Point2D pt, Rect circ);
        static bool isPtInRotatedRect(Point2D pt, Point2D A, Point2D B, Point2D D);
        static bool isPtInTriangle(Point2D pt, Point2D A, Point2D B, Point2D C);

        /* Circle and Polygon collision detection which also works while both shapes are rotating
         * rot_angle is needed to get the polygons current rotation angle */
        static bool isCircleIntersPolygon(Rect circle, float rot_angle, std::vector<float> vert);

        /* Can check either flat regions or corner regions */
        static bool isCircleInRegion(Rect circle, unsigned region, CollisionData *data);

        /* Called when a collision has occured. Determines collision data such as the region.
         * If this isnt the first collision the current region can be passed to search the most likely regions first. */
        static void genCollisionData(Rect circle, CollisionData *data, Dir facing, int cur_region =-1);
};

#endif /* COLLISION_H */

