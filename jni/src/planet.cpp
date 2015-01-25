#include <cmath>
#include "planet.h"
#include "colour.h"

const int Planet::_SIDES(60);
const int Planet::_GRAV_SIDES(70);
const float Planet::_GRAV_OPACITY(1.0f);

Planet::Planet(float x, float y, float d) : Object(x,y,d,d), _action(STILL) {
    // Redish-Brown colour
    _colour = Colour(0.258f, 0.258f, 0.258f, 1.0f);
    _rot_speed = 0.5f;
    _grav_r_off = d;
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

    _grav_r_off = (_grav_r_off <=0) ? getWidth() : 
                                      _grav_r_off-=3.5f;

    // Render Gravity area with rings
    for (int i=0; i<3; i++) {
        float offset = 80.0f * i;
        float radius = _grav_r_off - offset;
        float alpha = (radius > getWidth()/2) ? 2 - radius/(getWidth()/2) : 
                                                radius/(getWidth()/2);
        alpha *= _GRAV_OPACITY;

        // Render Gravity ring
        if (radius > 0)
            rend->render(getVerticeData(grav_vertex_count, radius),
                    Colour::getColourData(grav_vertex_count, Colour(_colour.r, _colour.g, _colour.b, alpha)), 
                    getRotAngle(), 
                    GL_LINE_LOOP);
    }
}

void Planet::update() {

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
        //
        // Rotate
        float objAngle = getRotAngle();

        float rad_angle = objAngle*PI/180.0;
        float rot_x =  x*cos(rad_angle) + y*sin(rad_angle);
        float rot_y = -x*sin(rad_angle) + y*cos(rad_angle);

        x = rot_x;
        y = rot_y;

        float percent = (i / (float) (vertex_count));
        float rad = percent * 2*PI;

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
