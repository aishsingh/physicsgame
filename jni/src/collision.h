/* Description: Collision class used to determine colisions
 * Created On: 2015-01-02 
 */

#ifndef COLLISION_H
#define COLLISION_H

#include "rect.h"
#include "point2d.h"

class Collision {
    public:
        static bool isBoundingBox(Rect box1, Rect box2);
        static bool isPtInRect(Point2D pt, Rect rect);
        static bool isPtInRect(int x, int y, Rect rect);
        static bool isCircleIntersCircle(Rect circ1, Rect circ2);
        static bool isPtInCircle(Point2D pt, Rect circ);
};

#endif /* COLLISION_H */

