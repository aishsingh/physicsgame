/* Description: Commonly used Math functions
 * Created On: 2015-01-11 
 */

#ifndef MATH_H
#define MATH_H

#include <vector>
#include "object.h"
#include "point2d.h"

using std::vector;

class Math {
    public:
        /* Rotations */
        static Point2D rotatePt(Point2D pt, float angle);
        static Point2D rotateObj(Object *obj);

        /* Determine the dispacement between the two rects */
        static float getRectDispFromRect(Rect rect1, Rect rect2);

        /* Returns a value between two values decided by the ratio (0.0 - 1.0)
         - if ratio == 0, val = a
           if ratio == 1, val = b */
        static float getValueFromRatioBetweenValues(float a, float b, float ratio);

        /* Determine the H side of a triangle using the Pythagoras theorem: c^2 = a^2 + b^2 */
        static float getHypotenuse(float a, float b);

        /* Ensures angle is between the min - max degrees */
        static float normalizeAngle(float angle, float min, float max);

        static float genRand(float min, float max);

        static vector<float> genRandData(int count, float min, float max);

        static vector<float> offsetDataByRand(vector<float> data, float min, float max);

        static vector<float> offsetDataByData(vector<float> data, vector<float> offset);
};

#endif /* MATH_H */
