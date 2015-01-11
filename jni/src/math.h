/* Description: Commonly used Math functions
 * Created On: 2015-01-11 
 */

#ifndef MATH_H
#define MATH_H

class Math {
    public:
        /* Determine the H side of a triangle using the Pythagoras theorem: c^2 = a^2 + b^2 */
        static float getHypotenuse(float a, float b);

        /* Ensures angle is between the min - max degrees */
        static float normalizeAngle(float angle, float min, float max);
};

#endif /* MATH_H */
