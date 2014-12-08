#ifndef BOX_H
#define BOX_H

#include "shape.h"

class Box: public Shape {
    private:
        Colour _colour;

    public:
        Box(float x, float y, float angle, float diameter, Theme &theme);
};

#endif /* BOX_H */
