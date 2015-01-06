#include <math.h>
#include "player.h"
#include "physics.h"

Player::Player(float x, float y, float width, float height) : Object(x,y,width,height) {
}

void Player::setBasePoint(Point2D point) {
    _base = point;
}

vector<float> Player::getVerticeData() {
    /*  [p1]----[p3]   
         |        |  
         |        |
         |        |
         |(player)|
         |        |
         |        |
         |        |
        [p2]----[p4]  */


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

    return std::vector<float> (vec, vec + sizeof(vec) / sizeof(float));
}

std::vector<float> Player::getColourData() {
    Colour colour(1.0f, 1.0f, 1.0f, 1.0f);

    float clr[] = { 0.3f    , 0.3f    , 0.3f    , colour.a,
                    colour.r, colour.g, colour.b, colour.a,
                    0.3f    , 0.3f    , 0.3f    , colour.a,
                    colour.r, colour.g, colour.b, colour.a};

    return std::vector<float> (clr, clr + sizeof(clr) / sizeof(float));
}

void Player::draw(AssetRenderer* _ass_rend) {
    // Render
    vector<float> vert = getVerticeData();
    float rot_angle = getRotAngle();
    _ass_rend->render(vert, getColourData(), rot_angle, GL_TRIANGLE_STRIP);

    /* Set base value (bottom of the player after rotation)
     * These calculations ensure that the base pos is the same as it would be after it went through the vshader.
     * The vshader rotates the pos, so the same rotation is applyed here */
    Point2D afterRot((vert[2] + vert[6])/2, (vert[3] + vert[7])/2);
    float rad_angle = getRotAngle()*PI/180.0;
    setBasePoint(Point2D(afterRot.getX()*cos(rad_angle) - afterRot.getY()*sin(rad_angle), 
                         afterRot.getY()*cos(rad_angle) + afterRot.getX()*sin(rad_angle)));

    // Attributes need to be disabled to avoid different shaders from reading in random values
    _ass_rend->disableAttributes();
}

void Player::applyGravity(vector<Planet*> *g_objs) {
    PhysicsEngine::applyGravityTo(*this, g_objs);
}

void Player::resetTime() {
    vert_motion.setTime(0);
    hori_motion.setTime(0);
}
