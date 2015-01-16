#include <stdlib.h>    // Needed for rand()
#include <math.h>
#include "shape.h"
#include "game.h"
#include "log.h"

#define PI 3.14159265358979323846264

Shape::Shape(float x, float y, int index, float angle, float width, float height, Theme &theme) : Object(x,y,width,height) {
    float time = Game::getElapsedTime();
    vert_motion.setTime(time);
    hori_motion.setTime(time);
    setIndex(index);
    setRotAngle(angle);
    useTheme(theme);
}

Colour Shape::getColour() const {
    return _colour;
}

void Shape::useTheme(Theme &theme) {
    switch(theme) {
        case GRAY:
        _colour.r = (256 - (rand() % 100)) / 256.0f;
        _colour.g = _colour.r;
        _colour.b = _colour.r;
        break;

        case RED:
        _colour.r = (rand() % 256) / 256.0f;
        _colour.g = (rand() % 100) / 256.0f;
        _colour.b = _colour.g;
        break;

        case PURPLE:
        _colour.g = (rand() % 50)  / 256.0f;
        _colour.b = (rand() % 256) / 256.0f;
        _colour.r = _colour.b/2;
        break;

        case BLUE:
        _colour.b = (rand() % 255)   / 256.0f;
        _colour.r = (_colour.b/6)    / 256.0f;
        _colour.g = (_colour.b/1.35) / 256.0f;
        break;

        case RAINBOW:
        _colour.r = (rand() % 256) / 256.0f;
        _colour.g = (rand() % 256) / 256.0f;
        _colour.b = (rand() % 256) / 256.0f;
        break;
    }

    // Reset Alpha value
    _colour.a = 1.0f;
}
float Shape::getAlpha() const {
    return _colour.a;
}
void Shape::setAlpha(float alpha) {
    _colour.a = alpha;
}

int Shape::getIndex() const {
    return _index;
}
void Shape::setIndex(const int index) {
    _index = index;
}

void Shape::draw(ObjRenderer *rend, Camera *cam) {
    // Render
    vector<float> vert = getVerticeData();
    rend->render(vert, 
                 Colour::getColourData(vert.size(), 
                 _colour), 
                 getRotAngle(), 
                 GL_TRIANGLE_STRIP,
                 cam);
}

vector<float> Shape::getVerticeData() {
    /*  [p1]---[p3]   
         |       |  
         | (box) |
         |       |
        [p2]---[p4]  */


    /* This is the original (x,y) that will now be transformed
     * before being passed to the vertex shader */
    float x = getX();
    float y = getY();
    float w = getWidth();
    float h = getHeight();

    // Translate to center
    x += (w/2);
    y += (h/2);

    // Rotate
    float objAngle = getRotAngle();

    float rad_angle = objAngle*PI/180.0;
    float rot_x =  x*cos(rad_angle) + y*sin(rad_angle);
    float rot_y = -x*sin(rad_angle) + y*cos(rad_angle);

    x = rot_x;
    y = rot_y;

    // Translate back to origin
    x -= (w/2);
    y -= (h/2);
    
    // Declare points (x,y)
    float vec[] = { x     , y     ,
                    x     , y + h ,
                    x + w , y     ,
                    x + w , y + h };

    return vector<float> (vec, vec + sizeof(vec) / sizeof(float));
}
