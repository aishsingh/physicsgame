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

        // The more sides (or vertices) the more round the object looks
        static const int _SIDES;
        static const int _GRAV_SIDES;
        static const float _GRAV_OPACITY;

        float _grav_r_off;

        Action _action;
        Colour _colour;
        float _rot_speed;

        vector<float> getVerticeData(int vertex_count, float r_offset);

    public:
        // Render planet
        virtual void draw(ObjRenderer *rend);
        virtual void drawGrav(ObjRenderer *rend);
        // Update player based on its current action
        virtual void update(); 

        /* Ctor - */
        Planet(float x, float y, float d);
        virtual ~Planet();
};

#endif /* PLANET_H */
