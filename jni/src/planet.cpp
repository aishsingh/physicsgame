#include <math.h>
#include "planet.h"

Planet::Planet(float x, float y, float d, float m) : Object(x,y,d,d) {
    _colour = Colour(0.647059f, 0.164706f, 0.164706f, 0.9f);
    _action = STILL;
    _mass = m;
}

float Planet::getMass() const {
    return _mass;
}

void Planet::draw(ObjRenderer *rend) {

    // Render Gravity area
    rend->render(getVerticeData(10, getWidth()/2), getColourData(10, Colour(_colour.r, _colour.g, _colour.b, 0.1f)), getRotAngle(), GL_TRIANGLE_FAN);

    // Render circle
    rend->render(getVerticeData(18, 0), getColourData(18), getRotAngle(), GL_TRIANGLE_FAN);

    setRotAngle(getRotAngle() + 0.5f);
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

vector<float> Planet::getColourData(int vertex_count) {
    Colour colour = _colour;

    vector<float> clr;
    for (int i=0; i<=vertex_count; i++) {
        clr.push_back(colour.r);
        clr.push_back(colour.g);
        clr.push_back(colour.b);
        clr.push_back(colour.a);
    }

    return clr;
}

vector<float> Planet::getColourData(int vertex_count, Colour col) {
    Colour colour = col;

    vector<float> clr;
    for (int i=0; i<=vertex_count; i++) {
        clr.push_back(colour.r);
        clr.push_back(colour.g);
        clr.push_back(colour.b);
        clr.push_back(colour.a);
    }

    return clr;
}
