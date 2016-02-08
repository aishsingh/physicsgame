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

// struct CollisionData {
//     Planet* planet;
//     int collision_side;
// };

class Collision {
    public:
        static bool isBoundingBox(Rect box1, Rect box2);
        static bool isPtInRect(Point2D pt, Rect rect);
        static bool isPtInRect(int x, int y, Rect rect);
        static bool isCircleIntersCircle(Rect circ1, Rect circ2);
        static bool isPtInCircle(Point2D pt, Rect circ);

        /* Circle and Polygon collision detection which also works while both shapes are rotating
         * rot_angle is needed to get the polygons current rotation angle */
        static bool isCircleIntersPolygon(Rect circle, float rot_angle, std::vector<float> vertices, int *collided_region = NULL, Point2D *collided_unit_vector = NULL);
};

#endif /* COLLISION_H */

