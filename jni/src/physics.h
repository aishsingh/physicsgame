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

namespace PhysicsEngine {
    /* Recalculate the motion and return the new motion values.
       NOTE: Fn doesn't modify the motion passed through params, it only needs to be validated */
    Motion calcMotion(const Motion &motion);

    /* Updates all physics motion values and returns a gravobject if the obj collided with it */
    GravObject* updatePhysicsForCollisions(Object *obj, const vector<GravObject*> *g_objs);

    /* Generated a suitable initial velocity using based on the angle the object is rotated in */
    void genInitVel(Object &obj, float rot_angle, float min, float max, float offset);

    unsigned getQuadOfPtAroundRect(Point2D pt, Rect rect);
    /* Gets an angle calculated from the distance to a point inside of an object. 
       Angle starts at the middle-top (0*C) and increases anti-clockwise */
    float getAngleOfPtAroundRect(Point2D pt, Rect rect);

    /* Split vertical and horizontal component values based on an angle. 
       The split needs to know the min & max values that should occur for both components */
    void splitValueFromAngle(float value, float angle, float *hori, float *vert);
    void splitCompValueFromAngle(float *hori_comp, float *vert_comp, float angle, float min_hori, float max_hori, float min_vert, float max_vert);

    void updatePlayerOrbittingPlanets(Player &player, const vector<GravObject*> *g_objs);

    /* Updates object vert/hori acceleration due to the gravity from the given planets */
    void applyGravityTo(Player &player, const vector<GravObject*> *g_objs);
    void applyGravityTo(Object &obj, const vector<GravObject*> *g_objs);
}

#endif /* PHYSICS_H */

