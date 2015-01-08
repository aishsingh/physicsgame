/* Description: Camera class is used by renderers to set the View Matrix
 * Created On: 2015-01-08 
 */

#include "object.h"
#include "point2d.h"

#ifndef CAMERA_H
#define CAMERA_H

class Camera {
    private:
        const Object *_anchor;
        Point2D _offset;

    public:
        /* Mutators */
        void setAnchorObj(const Object *obj);
        void setOffset(Point2D offset);

        /* Accessors */
        Point2D getPos() const;
        float getRotAngle() const;

        /* Ctor */
        Camera(const Object *anchor);
};

#endif /* CAMERA_H */
