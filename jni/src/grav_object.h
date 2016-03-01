/* Description: Gravity Objects allow players to land on them
 * Created On: 2016-02-18
 */

#ifndef GRAV_OBJECT_H
#define GRAV_OBJECT_H

#include "object.h"
#include "obj_renderer.h"

class GravObject: public Object {
    protected:
        float _rot_speed;
        float _grav_radius_offset; // The extra radius used by the gravity area
        float _grav_speed;         // The speed the rings shrink inwards
        float _grav_rings_off[3];  // The current radius offset for each of the gravity rings

    public:
        virtual void draw(ObjRenderer *rend) = 0;
        virtual void drawStats(ObjRenderer *rend, bool on_planet, int collided_region) = 0;
        void drawGrav(ObjRenderer *rend);

        virtual vector<float> getVertices() const = 0;
        virtual float getRegionAngleOffset(int region) const = 0;
        void anchorObject(Object *obj);
        float getRadiusOffset() const;
        float getRotSpeed() const;

        /* Ctor / Dtor */
        GravObject(float x, float y, float width, float height, float rot_angle=0.0f);
        virtual ~GravObject();
};

#endif /* GRAV_OBJECT_H */
