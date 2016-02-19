/* Description: Physics functions that can be used staticly
 * Created On: 2014-08-16 
 */


#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>
#include "object.h"
#include "grav_object.h"
#include "planet.h"
#include "player.h"
#include "rect.h"
#include "point2d.h"
#include "collision.h"

using std::vector;

class PhysicsEngine {
    private:
        /* Recalculate the motion and return the new motion values.
           NOTE: Fn doesn't modify the motion passed through params as it needs to be validated */
        static Motion calcMotion(const Motion &motion);

    public:
        /* Updates all motion and returns true if the object landed on a planet.
           data param: Non-complex objects such as the boxes in a players trail only need to know if a collision occured, so this param is optional */
        static void updatePhysics(Object *obj, const vector<GravObject*> *g_objs, CollisionData *data = NULL);

        /* Generated a suitable initial velocity using based on the angle the object is rotated in */
        static void genInitVel(Object &obj, float rot_angle, float min, float max, float offset);

        /* Gets an angle calculated from the distance to a point inside of an object. 
           Angle starts at the middle-top (0*C) and increases anti-clockwise */
        static float getAngleOfPtFromRectCentre(Point2D pt, Rect rect);

        /* Split vertical and horizontal component values based on an angle. 
           The split needs to know the min & max values that should occur for both components */
        static void splitValueFromAngle(float value, float angle, float *hori, float *vert);
        static void splitCompValueFromAngle(float *hori_comp, float *vert_comp, float angle, float min_hori, float max_hori, float min_vert, float max_vert);

        static void updatePlayerOrbittingPlanets(Player &player, const vector<GravObject*> *g_objs);

        /* Updates object vert/hori acceleration due to the gravity from the given planets */
        static void applyGravityTo(Player &player, const vector<GravObject*> *g_objs);
        static void applyGravityTo(Object &obj, const vector<GravObject*> *g_objs);
};

#endif /* PHYSICS_H */

