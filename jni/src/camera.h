/* Description: Camera class is used by renderers to set the View Matrix
 * Created On: 2015-01-08 
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "object.h"
#include "point2d.h"

#define CAM_RotateViewToFollowPlayer true

class Camera {
    private:
        const Object *_anchor;
        Point2D _pos_offset;
        float _rot_offset_angle;
        float _scale;

    public:
        /* Mutators */
        void setAnchorObj(const Object *obj);
        void setPosOffset(Point2D offset);
        void setRotAngleOffset(float offset);
        void setScaleFromDisp(float d);

        /* Accessors */
        Point2D getPos();
        float getRotAngle() const;
        float getScale() const;

        /* Ctor */
        Camera(const Object *anchor);

        bool isObjectInCam(Object *obj);
        bool isPtInCam(Point2D pt, float offset);
};

#endif /* CAMERA_H */
