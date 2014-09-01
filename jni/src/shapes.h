#ifndef SHAPES_H
#define SHAPES_H

#include "object.h"
#include "motion.h"

class Box: public Object {
    private:
        Colour _colour;
    public:
        Motion vert_motion, hori_motion;

        float rot_angle;

        /* Getters / Setters */
        Colour getColour();
        void setColour(const Theme &theme);
        float getAlpha();
        void setAlpha(const float alpha);

        /* Ctor
           Check if there is once 
           Creates a new box instance and adds it to the array */
        Box() {}
        Box(const float x, const float y, const float diameter, const int &index, const float &time, const Theme &theme);
};

#endif /* SHAPES_H */
