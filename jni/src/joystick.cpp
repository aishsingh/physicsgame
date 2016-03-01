#include <math.h>
#include "joystick.h"
#include "game.h"
#include "physics.h"
#include "log.h"

Joystick::Joystick(float x, float y, float l, float size, float origin_size) : Rect(x,y,l,l), _size(size), _ori_size(origin_size) { }

void Joystick::draw(ScreenRenderer *rend) {
    // Render
    vector<float> vert = getVerticeData();
    rend->render(vert, Colour::getColourData(vert.size(), Colour(1.0f, 0.0f, 0.0f, 1.0f)), 0, GL_LINES);
}

vector<float> Joystick::getVerticeData() {
    /*     [p3] [p4]          Origin Centre
              ---             
        [p1]       [p5]          [p11]
           |   +   |     ---> [p9] + [p10]
        [p2]       [p6]          [p12]
              ---            
           [p8] [p7]                      */


    /* This is the original (x,y) that will now be transformed
     * before being passed to the vertex shader */
    float x = getX();
    float y = getY();
    float l = getWidth();
    float offset = (l - _size)/2;
    
    // Declare points (x,y)
    float vec[] = { x                         , y + offset                ,
                    x                         , y + _size + offset        ,
                    x + offset                , y                         ,
                    x + _size + offset        , y                         ,
                    x + l                     , y + offset                ,
                    x + l                     , y + _size + offset        ,
                    x + offset                , y + l                     ,
                    x + _size + offset        , y + l                     ,
                    x + (l/2) - (_ori_size/2) , y + (l/2)                 ,
                    x + (l/2) + (_ori_size/2) , y + (l/2)                 ,
                    x + (l/2)                 , y + (l/2) - (_ori_size/2) ,
                    x + (l/2)                 , y + (l/2) + (_ori_size/2) };

    return std::vector<float> (vec, vec + sizeof(vec) / sizeof(float));
}

float Joystick::getJoystickAngle(int x, int y) {
    return PhysicsEngine::getAngleOfPtAroundRect(Point2D(x, y), *this);
}

float Joystick::getJoystickAngle(Point2D pos, float obj_rot_angle) {
    float angle = PhysicsEngine::getAngleOfPtAroundRect(pos, *this);
    // angle += obj_rot_angle;
    // LOGI("angle %.2f", angle);
    return angle;
}
