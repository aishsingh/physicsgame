/* Description: Physics functions that can be used staticly
 * Created On: 2014-08-16 
 */


#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>
#include "object.h"
#include "planet.h"
#include "rect.h"
#include "point2d.h"

using std::vector;

class PhysicsEngine {
    private:
        /* Pushes new boxes out with some initial velocity 
           - up is neg, down is pos 
           - up is left, right is pos */
        static const float _MAX_INIT_V;
        static const float _MIN_INIT_V;
        static const float _INIT_V_OFFSET;

        // Creates a more natural look of gravity when gravity is switched up/down
        static const float _MAX_G_SWITCH_MISCALC;

        /* Recalculate the motion and return the new motion values.
           NOTE: Fn doesn't modify the motion passed through params as it needs to be validated */
        static Motion calcMotion(const Motion &motion);

    public:
        static bool updatePhysics(Object &obj, vector<Planet*> *g_objs);

        /* Reverse gravity and give blocks some initial velocity to look natural
           1. Invert gravity value
           2. Reset block time
           3. give blocks some initial velocity to make blocks fall with different levels
        void switchGravity(Object objs[], const int &boxes_count, const int &elapsed_time);
        */

        /* Generated a suitable initial velocity using based on the angle the object is rotated in */
        static void genInitVel(Object &obj, float rot_angle, float min, float max, float offset);

        /* Gets an angle calculated from the distance to a point inside of an object. 
           Angle starts at the middle-top (0*C) and increases anti-clockwise */
        static float getAngleOfPtFromRectCentre(Point2D pt, Rect rect);

        /* Split vertical and horizontal component values based on an angle. 
           The split needs to know the min & max values that should occur for both components */
        static void splitValueFromAngle(float value, float angle, float *hori, float *vert);
        static void splitCompValueFromAngle(float *hori_comp, float *vert_comp, float angle, float min_hori, float max_hori, float min_vert, float max_vert);

        /* Updates object vert/hori acceleration due to the gravity from the given planets */
        static void applyGravityTo(Object &obj, vector<Planet*> *g_objs);
        static void applyGravityTo(Object &obj, vector<Planet*> *g_objs, Camera *cam);
};

#endif /* PHYSICS_H */

