#include <cmath>
#include "grav_object.h"
#include "config.h"
#include "math.h"
#include "shape.h"
#include "log.h"

GravObject::GravObject(float x, float y, float width, float height, float rot_angle) : Object(x,y,width,height,rot_angle) {
    _rot_speed = (GOBJ_ROTATE) ? powf(0.4f, (getWidth()/200)) : 0.0f;
    _grav_radius_offset = 120.0f;
    _grav_speed = 5.5f;

    // Position grav rings
    _grav_rings_off[0] = width;
    for (int i=1; i<=2; i++) {
        float offset = _grav_radius_offset / i;
        _grav_rings_off[i] = width + offset;
    }
}
GravObject::~GravObject() { }

void GravObject::drawGrav(ObjRenderer *rend) {
    // Render gravity area
    vector<float> vert = Shape::genCircleVertices(getCentre(), getWidth()*1.5, getRotAngle(), 50);
    rend->render(vert,
                 Colour(1.0f, 1.0f, 1.0f, 0.03f),
                 getRotAngle(), 
                 GL_TRIANGLE_FAN);

    rend->render(vert,
                 Colour(1.0f, 1.0f, 1.0f, 0.15f),
                 getRotAngle(), 
                 GL_LINE_LOOP);


    // Render all gravity rings
    int grav_vertex_count = GOBJ_AVERAGE_SIDES * (getWidth()/400);
    int vertex_count = 15 * (getWidth()/400);
    int lines = (vertex_count <= 7) ? 2:3;

    for (int i=0; i<lines; i++) {
        _grav_rings_off[i] = (_grav_rings_off[i] <=0) ? getWidth() : _grav_rings_off[i]-=_grav_speed;

        float alpha = (_grav_rings_off[i] > getWidth()/2) ? 2 - _grav_rings_off[i]/(getWidth()/2) : _grav_rings_off[i]/(getWidth()/2);
        alpha *= GOBJ_OPACITY;

        if (_grav_rings_off[i] > 0) {
            float radius = (getWidth()/2) + _grav_rings_off[i];
            vector<float> vert = Shape::genCircleVertices(getCentre(), radius, getRotAngle(), grav_vertex_count);

            Colour c = PLANET_COLOUR;
            c.a = fabs(alpha);
            rend->render(Math::offsetDataByRand(vert, -8.0f, 8.0f),
                         c,
                         getRotAngle(), 
                         GL_LINE_LOOP);
        }
    }
}

void GravObject::anchorObject(Object *obj) {
    Point2D pt = Math::rotatePtAroundPt(getCentre(), obj->getCentre(), _rot_speed);

    obj->setX(pt.getX() - (obj->getWidth()/2));
    obj->setY(pt.getY() - (obj->getHeight()/2));
}

float GravObject::getRadiusOffset() const {
    return _grav_radius_offset;
}

float GravObject::getRotSpeed() const {
    return _rot_speed;
}
