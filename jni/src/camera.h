/* Description: Camera class is used by renderers to set the View Matrix
 * Created On: 2015-01-08 
 */

#include "object.h"
#include "point2d.h"

#ifndef CAMERA_H
#define CAMERA_H

#define CAM_RotateViewToFollowPlayer true

class Camera {
    private:
        const Object *_anchor;
        Point2D _pos_offset;
        float _rot_offset_angle;

    public:
        /* Mutators */
        void setAnchorObj(const Object *obj);
        void setPosOffset(Point2D offset);
        void setRotAngleOffset(float offset);

        /* Accessors */
        Point2D getPos();
        float getRotAngle() const;

        /* Ctor */
        Camera(const Object *anchor);
};

#endif /* CAMERA_H */
