#include <math.h>
#include "planet.h"

Planet::Planet(float x, float y, float d) : Object(x,y,d,d) {
    _colour = Colour(0.647059f, 0.164706f, 0.164706f, 1.0f);
    _action = Action::STILL;
    setRotAngle(0.0f);
    _gravity = 0.98f; // TODO should be determined from width
}

float Planet::getGravity() {
    return _gravity;
}

void Planet::draw(ObjRenderer *rend) {
    vector<float> ver = getVerticeData();
    // Render
    rend->render(ver, getColourData(ver.size()/2), getRotAngle(), GL_TRIANGLE_FAN);
}

void Planet::update() {

}

vector<float> Planet::getVerticeData() {
    /*    ___
         /    \
        (Circle) 
         \____/
                  */

int vertexCount = 30;
float radius = getWidth()/2;
float center_x = getX();
float center_y = getY();

//create a buffer for vertex data
vector<float> vec;// = vertexCount*2; // (x,y) for each vertex
int idx = 0;

//center vertex for triangle fan
vec.push_back(center_x);
vec.push_back(center_y);

//outer vertices of the circle
int outerVertexCount = vertexCount-1;

for (int i = 0; i < outerVertexCount; ++i){
    float percent = (i / (float) (outerVertexCount-1));
    float rad = percent * 2*PI;

    //vertex position
    float outer_x = center_x + radius * cos(rad);
    float outer_y = center_y + radius * sin(rad);

    vec.push_back(outer_x);
    vec.push_back(outer_y);
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

