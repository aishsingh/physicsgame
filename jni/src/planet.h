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

        static const int _SIDES;       // The more sides (or vertices) the more round the object looks
        static const bool _RAND_SIDES; // The sides will not be symetrical if true
        static const bool _DRAW_NORMALS;

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
