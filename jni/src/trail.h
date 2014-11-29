/* Description: Manage the player trail
 * Created On: 2014-09-01 
 */

#ifndef TRAIL_H
#define TRAIL_H

#include <vector>
#include "shape.h"
#include "physics.h"
#include "rend_box.h"

class Trail {
    private:
        int _boxes_length;

        Rend_box _renderer;

        // fade the selected box
        void fade(Shape &shape);
    public:
        // Shapes that are displayed in the trail
        std::vector<Shape> shapes;

        /* Draw the shapes to the renderer */
        void render(PhysicsEngine &physics, int time, int screen_width, int screen_height);

        /* Creates a new box with the given values
           1. Initializes a bew instanse  */
        void createBox(float x, float y, float time, Theme theme, PhysicsEngine &physics);

        /* Removes the box at the given index
           1. Shifts boxes that are right of the index one pos left 
           2. Decreases the memory allocation of the array */
        void removeBox(int index);

        // Rotate the entire 
        void rotate(float angle);

        void setup(int screen_w, int screen_h);

        /* Ctor */
        Trail();

        /* Dtor */
        ~Trail();
};

#endif /* TRAIL_H */
