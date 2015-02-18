#include <stdlib.h>    // Needed for rand()
#include <math.h>
#include "shape.h"
#include "game.h"
#include "math.h"

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
        _colour.g = _colour.r/2.6f;
        _colour.b = _colour.g;
        break;

        case PURPLE:
        _colour.b = (rand() % 256) / 256.0f;
        _colour.r = _colour.b/4;
        _colour.g = _colour.b/5.2f;
        break;

        case BLUE:
        _colour.b = (rand() % 256) / 256.0f;
        _colour.r = _colour.b/4;
        _colour.g = _colour.b/1.5f;
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

void Shape::draw(ObjRenderer *rend) {
    // Render
    rend->render(getVerticeData(), 
                 _colour, 
                 getRotAngle(), 
                 GL_TRIANGLE_STRIP);
}

vector<float> Shape::getVerticeData() {
    /*  [p1]---[p3]   
         |       |  
         | (box) |
         |       |
        [p2]---[p4]  */

    // This is the new pos (x,y) after being transformed
    Point2D pt = Math::rotateObj(this);

    float x = pt.getX();
    float y = pt.getY();
    float w = getWidth();
    float h = getHeight();
    
    // Declare points (x,y)
    float vec[] = { x     , y     ,
                    x     , y + h ,
                    x + w , y     ,
                    x + w , y + h };

    return vector<float> (vec, vec + sizeof(vec) / sizeof(float));
}
