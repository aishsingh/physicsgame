#ifndef BOX_H
#define BOX_H

#include "shape.h"

class Box: public Shape {
    public:
        Box(float x, float y, int index, float angle, float diameter, Theme &theme);
};

#endif /* BOX_H */
