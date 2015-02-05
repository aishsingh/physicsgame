#include <cmath>
#include "planet.h"
#include "colour.h"
#include "math.h"

const int Planet::_SIDES(60);
const int Planet::_GRAV_SIDES(70);
const float Planet::_GRAV_OPACITY(0.3f);

Planet::Planet(float x, float y, float d) : Object(x,y,d,d), _action(STILL) {
    _colour = Colour(0.9294f, 0.898f, 0.88627f, 1.0f);
    _rot_speed = 0.5f;
    _radius_offset = getWidth()/5;

    // Position grav rings
    for (int i=1; i<=3; i++) {
        float offset = _radius_offset / i;
        _grav_r_off[i] = d + offset;
    }
}
Planet::~Planet() { }

void Planet::draw(ObjRenderer *rend) {
    // Render circle
    float vertex_count = _SIDES * getWidth()/400;
    rend->render(getVerticeData(vertex_count, 0), 
                 Colour::getColourData(vertex_count, _colour), 
                 getRotAngle(), 
                 GL_TRIANGLE_FAN);

    // Rotate
    // setRotAngle(getRotAngle() + _rot_speed);
}

void Planet::drawGrav(ObjRenderer *rend) {
    float grav_vertex_count = _GRAV_SIDES * (getWidth()/400);

    // Render Gravity area with rings
    for (int i=0; i<3; i++) {
        _grav_r_off[i] = (_grav_r_off[i] <=0) ? getWidth() : 
                                                _grav_r_off[i]-=3.0f;

        float alpha = (_grav_r_off[i] > getWidth()/2) ? 2 - _grav_r_off[i]/(getWidth()/2) : 
                                                        _grav_r_off[i]/(getWidth()/2);
        alpha *= _GRAV_OPACITY;

        // Render Gravity ring
        if (_grav_r_off[i] > 0)
            rend->render(getVerticeData(grav_vertex_count, _grav_r_off[i]),
                    Colour::getColourData(grav_vertex_count, Colour(_colour.r, _colour.g, _colour.b, alpha)), 
                    getRotAngle(), 
                    GL_LINE_LOOP);
    }
}

void Planet::update() { }

float Planet::getRadiusOffset() const {
    return _radius_offset;
}

vector<float> Planet::getVerticeData(int vertex_count, float r_offset) {
     /*    ___
          /    \
         (Circle) 
          \____/
                    */

    float radius = (getWidth()/2) + r_offset;
    float c_x = getCentreX();
    float c_y = getCentreY();

    //create a buffer for vertex data
    vector<float> vec; // (x,y) for each vertex
    int idx = 0;

    for (int i = 0; i < vertex_count; ++i){

        float x = c_x;
        float y = c_y;

        // Rotate
        Point2D pt = Math::rotatePt(Point2D(x, y), getRotAngle());
        x = pt.getX();
        y = pt.getY();

        float percent = (i / (float) (vertex_count));
        float rad = percent * 2 * PI;

        //vertex position
        x += radius * cos(rad);
        y += radius * sin(rad);

        // Add pos
        vec.push_back(x);
        vec.push_back(y);
    }
    //create VBO from buffer with glBufferData()

    return vec;
}
