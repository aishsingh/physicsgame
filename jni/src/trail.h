/* Description: Manage the player trail
 * Created On: 2014-09-01 
 */

#ifndef TRAIL_H
#define TRAIL_H

#include <vector>
#include "shape.h"
#include "grav_object.h"
#include "obj_renderer.h"

class Trail {
    private:
        float _boxes_length;
        Colour _cur_colour;

        void fade(Shape &shape);
        void shrink(Shape &shape);

    public:
        // Shapes that are displayed in the trail
        std::vector<Shape> shapes;

        /* Draw the shapes to the renderer */
        void draw(ObjRenderer *rend, const vector<GravObject*> *g_objs);

        /* Creates a new box with the given values
           1. Initializes a bew instanse  */
        void buildTrail(float x, float y, float rot_angle, Theme theme);

        void applyGravity(const vector<GravObject*> *g_objs);
        void resetTime(float t);

        /* Removes the box at the given index
           1. Shifts boxes that are right of the index one pos left 
           2. Decreases the memory allocation of the array */
        void removeBox(int index);

        /* Used by player renderer */
        Colour getCurColour() const;

        /* Ctor / Dtor */
        Trail(int obj_length);
        ~Trail();
};

#endif /* TRAIL_H */
