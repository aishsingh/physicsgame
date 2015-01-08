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
    float rad_angle = getRotAngle()*PI/180;
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

    /* Set base value (bottom of the player after rotation)
     * These calculations ensure that the base pos is the same as it would be after it went through the vshader.
     * The vshader rotates the pos, so the same rotation is applyed here */
    Point2D mid_bottom((vec[2] + vec[6])/2, (vec[3] + vec[7])/2);
    setBasePoint(Point2D(mid_bottom.getX()*cos(rad_angle) - mid_bottom.getY()*sin(rad_angle), 
                         mid_bottom.getY()*cos(rad_angle) + mid_bottom.getX()*sin(rad_angle)));

    return std::vector<float> (vec, vec + sizeof(vec) / sizeof(float));
}

std::vector<float> Player::getColourData() {
    float clr[] = { 0.3f, 0.3f, 0.3f, 1.0f,
                    1.0f, 1.0f, 1.0f, 1.0f,
                    0.3f, 0.3f, 0.3f, 1.0f,
                    1.0f, 1.0f, 1.0f, 1.0f};

    return std::vector<float> (clr, clr + sizeof(clr) / sizeof(float));
}

void Player::draw(AssetRenderer* _ass_rend, Camera *cam) {
    // Render
    _ass_rend->render(getVerticeData(), 
                      getColourData(), 
                      getRotAngle(), 
                      GL_TRIANGLE_STRIP,
                      cam);

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
