#include <cmath>
#include "grav_object.h"
#include "math.h"
#include "shape.h"
#include "log.h"

const int GravObject::_GRAV_SIDES(45);
const float GravObject::_GRAV_OPACITY(0.5f);

GravObject::GravObject(float x, float y, float width, float height, float rot_angle) : Object(x,y,width,height,rot_angle) {
    _rot_speed = 0.1f;//powf(0.4f, (getWidth()/200));
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
    int grav_vertex_count = _GRAV_SIDES * (getWidth()/400);
    int vertex_count = 15 * (getWidth()/400);
    int lines = (vertex_count <= 7) ? 2:3;

    // Render Gravity area with rings
    for (int i=0; i<lines; i++) {
        _grav_rings_off[i] = (_grav_rings_off[i] <=0) ? getWidth() : _grav_rings_off[i]-=_grav_speed;

        float alpha = (_grav_rings_off[i] > getWidth()/2) ? 2 - _grav_rings_off[i]/(getWidth()/2) : _grav_rings_off[i]/(getWidth()/2);
        alpha *= _GRAV_OPACITY;

        // Render Gravity ring
        if (_grav_rings_off[i] > 0) {
            float radius = (getWidth()/2) + _grav_rings_off[i];
            vector<float> vert = Shape::genCircleVertices(getCentre(), radius, getRotAngle(), grav_vertex_count);

            rend->render(Math::offsetDataByRand(vert, -8.0f, 8.0f),
                    Colour(Colour(0.9294f, 0.898f, 0.88627f, fabs(alpha))), 
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
