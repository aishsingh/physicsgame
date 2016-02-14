/* Description: Collision class used to determine colisions
 * Created On: 2015-01-02 
 */

#ifndef COLLISION_H
#define COLLISION_H

#include <vector>
#include "rect.h"
#include "point2d.h"
#include "object.h"
#include "planet.h"
#include "direction.h"

struct CollisionData {
    Planet* planet;
    int region;
    Point2D unit_vec;
    Dir facing;

    CollisionData() : planet(NULL), region(-1), facing(RIGHT) { }
};

class Collision {
    public:
        static bool isBoundingBox(Rect box1, Rect box2);
        static bool isPtInRect(Point2D pt, Rect rect);
        static bool isPtInRect(int x, int y, Rect rect);
        static bool isCircleIntersCircle(Rect circ1, Rect circ2);
        static bool isPtInCircle(Point2D pt, Rect circ);
        static bool isPtInRhombus(Point2D pt, Point2D A, Point2D B, Point2D C, Point2D D);

        /* Circle and Polygon collision detection which also works while both shapes are rotating
         * rot_angle is needed to get the polygons current rotation angle */
        static bool isCircleIntersPolygon(Rect circle, float rot_angle, std::vector<float> vertices, CollisionData *data = NULL);//, int *collided_region = NULL, Point2D *collided_unit_vector = NULL);
};

#endif /* COLLISION_H */

