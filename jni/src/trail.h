/* Description: Manage the player trail
 * Created On: 2014-09-01 
 */

#include <vector>
#include "shapes.h"
#include "physics.h"
#include "renderer.h"

class Trail {
    private:
        int _boxes_length;

        // fade the selected box
        void fade(Box &box);
    public:
        // Shapes that are displayed in the trail
        std::vector<Box> shapes;

        /* Draw the shapes to the renderer */
        void render(Renderer &renderer, PhysicsEngine &physics, int time, int screen_width, int screen_height);

        /* Creates a new box with the given values
           1. Initializes a bew instanse  */
        void createBox(float x, float y, float time, Theme theme, PhysicsEngine &physics);


        /* Removes the box at the given index
           1. Shifts boxes that are right of the index one pos left 
           2. Decreases the memory allocation of the array */
        void removeBox(int index);

        // Rotate the entire 
        void rotateBy(float angle);

        // Ctor
        Trail();
};
