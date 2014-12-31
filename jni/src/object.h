#ifndef OBJECT_H
#define OBJECT_H

#include "motion.h"

class Object {
    private:
        float _x, _y;
        float _width, _height;
        float _rot_angle;

    public:
        Motion vert_motion, hori_motion;

        /* Getters / Setters */
        float getX() const;
        void setX(float x);
        float getY() const;
        void setY(float y);
        float getWidth() const;
        float getHeight() const;
        void setLength(float l);
        float getRotAngle() const;
        void setRotAngle(float angle);
        float getCentreX() const;
        float getCentreY() const;

        /* Ctor */
        Object(float x, float y, float width, float height);

        /* Dtor */
        ~Object();
};

#endif /* OBJECT_H */
