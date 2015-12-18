/* Description: Planet header
 * Created On: 2014-12-17 
 */

#ifndef PLANET_H
#define PLANET_H

#include "object.h"
#include "obj_renderer.h"

class Planet: public Object {
    private:
        vector<float> _vertices;
        vector<float> _vertex_offsets;

        enum Action {
            STILL, 
            ROTATING, 
            BREAKING
        };

        // The more sides (or vertices) the more round the object looks
        static const int _SIDES;
        static const int _GRAV_SIDES;
        static const float _GRAV_OPACITY;
        static const bool _RAND_SIDES;    // The sides will not be symetrical if true
        static const bool _DRAW_NORMALS;  // The sides will not be symetrical if true

        float _grav_radius_offset; // The extra radius used by the gravity area
        float _grav_speed;         // The speed the rings shrink inwards
        float _grav_rings_off[3];  // The current radius offset for each of the gravity rings

        Action _action;
        Colour _colour;
        float _rot_speed;

        vector<float> getVerticeData(int vertex_count, float r_offset);

    public:
        // Render planet
        virtual void draw(ObjRenderer *rend);
        virtual void drawGrav(ObjRenderer *rend);
        void drawStats(ObjRenderer *rend, Rect circle);

        // Update player based on its current action
        virtual void update(); 

        void anchorObject(Object *obj, Point2D base);

        // Accessors
        float getRadiusOffset() const;
        vector<float> getVertices() const;

        /* Ctor */
        Planet(float x, float y, float d);
        virtual ~Planet();
};

#endif /* PLANET_H */
