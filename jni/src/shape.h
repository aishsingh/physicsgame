#ifndef SHAPE_H
#define SHAPE_H

#include "object.h"
#include "colour.h"
#include "theme.h"

class Shape: public Object {
    private:
        int _index;
        Colour _colour;

    public:
        /* Getters / Setters */
        Colour getColour() const;
        void useTheme(Theme &theme);
        float getAlpha() const;
        void setAlpha(float alpha);
        int getIndex() const;
        void setIndex(int index);

        /* Ctor - Creates a new box instance and adds it to the array */
        Shape(float x, float y, int index, float angle, float width, float height, Theme &theme);
};

#endif /* SHAPE_H */
