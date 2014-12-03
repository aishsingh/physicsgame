#ifndef SHAPE_H
#define SHAPE_H

#include "object.h"
#include "colour.h"
#include "theme.h"

class Shape: public Object {
    private:
        Colour _colour;

    public:
        /* Getters / Setters */
        Colour getColour() const;
        void useTheme(Theme &theme);
        float getAlpha() const;
        void setAlpha(float alpha);

        /* Ctor - Creates a new box instance and adds it to the array */
        // Box() {}
        Shape(float x, float y, float width, float height, Theme &theme);
};

#endif /* SHAPE_H */
