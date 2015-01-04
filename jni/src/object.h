/* Description: Objects class used for all game objects
 * Created On: 2014-08-16
 */

#ifndef OBJECT_H
#define OBJECT_H

#include "rect.h"
#include "motion.h"
#include "point2d.h"

class Object: public Rect {
    private:
        float _rot_angle;

    public:
        Motion vert_motion, hori_motion;

        float getRotAngle() const;
        void setRotAngle(float angle);
        Point2D getPos() const;

        /* Ctor */
        Object(float x, float y, float width, float height);

        /* Dtor */
        ~Object();
};

#endif /* OBJECT_H */
