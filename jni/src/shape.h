/* Description: Shape class header
 * Created On: 2014-11-30
 */
#ifndef SHAPE_H
#define SHAPE_H

#include "object.h"
#include "colour.h"
#include "theme.h"
#include "obj_renderer.h"
#include "camera.h"

class Shape: public Object {
    private:
        int _index;
        Colour _colour;

        vector<float> getVerticeData();

    public:
        /* Getters / Setters */
        Colour getColour() const;
        void useTheme(Theme &theme);
        float getAlpha() const;
        void setAlpha(float alpha);
        int getIndex() const;
        void setIndex(int index);
        void draw(ObjRenderer *rend, Camera *cam);

        /* Ctor - Creates a new box instance and adds it to the array */
        Shape(float x, float y, int index, float angle, float width, float height, Theme &theme);
};

#endif /* SHAPE_H */
