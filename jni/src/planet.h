/* Description: Planet header
 * Created On: 2014-12-17 
 */

#ifndef PLANET_H
#define PLANET_H

#include "grav_object.h"
#include "obj_renderer.h"
#include "colour.h"

class Planet: public GravObject {
    private:
        vector<float> _vertices;
        vector<float> _vertex_offsets;

        enum Action {
            STILL, 
            ROTATING, 
            BREAKING
        };

        Action _action;
        Colour _colour;

    public:
        // Render planet
        void draw(ObjRenderer *rend);
        void drawStats(ObjRenderer *rend, bool on_planet, int collided_region);

        // Accessors
        vector<float> getVertices() const;

        /* Ctor */
        Planet(float x, float y, float d);
        virtual ~Planet();
};

#endif /* PLANET_H */
