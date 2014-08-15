#ifndef OBJECT_H
#define OBJECT_H

#include "motion.h"
#include "common.h"

class Object {
    private:
        float _x, _y;
        int _index;
        float _width, _height;

    public:
        Motion vert_motion, hori_motion;

        /* Getters / Setters */
        float getX();
        void setX(const float x);
        float getY();
        void setY(const float y);
        int getIndex();
        void setIndex(const int index);
        float getWidth();
        float getHeight();

        /* Ctor */
        Object() {}
        Object(float x, float y, float width, float height);

        /* Dtor */
        ~Object();
};

#endif /* OBJECT_H */
