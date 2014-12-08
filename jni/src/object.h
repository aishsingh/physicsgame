#ifndef OBJECT_H
#define OBJECT_H

#include "motion.h"

class Object {
    private:
        float _x, _y;
        // int _index;
        float _width, _height;

    public:
        Motion vert_motion, hori_motion;
        float rot_angle; //FIME apply encapsulation

        /* Getters / Setters */
        float getX() const;
        void setX(float x);
        float getY() const;
        void setY(float y);
        // int getIndex();
        // void setIndex(const int index);
        float getWidth() const;
        float getHeight() const;

        /* Ctor */
        Object() {}
        Object(float x, float y, float width, float height);


        /* Dtor */
        ~Object();
};

#endif /* OBJECT_H */
