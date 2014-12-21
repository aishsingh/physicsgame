/* Description: Planet header
 * Created On: 2014-12-17 
 */

#ifndef PLANET_H
#define PLANET_H

#include "object.h"
#include "obj_renderer.h"

class Planet: public Object {
    protected:
        enum Action {
            STILL, 
            ROTATING, 
            BREAKING
        };

        Action _action;
        Colour _colour;

        vector<float> getVerticeData();
        vector<float> getColourData(int vertex_count);

    public:
        // Render planet
        virtual void draw(ObjRenderer *rend);
        // Update player based on its current action
        virtual void update(); 

        /* Ctor - */
        Planet(float x, float y, float d);
};

#endif /* PLANET_H */
