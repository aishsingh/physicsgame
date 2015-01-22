#include <math.h>
#include "player.h"
#include "physics.h"
#include "math.h"

Player::Player(float x, float y, float width, float height) : Object(x,y,width,height) {
    _rot_offset_angle = 0.0f;
    _on_planet = 0;
}
Player::~Player() { }

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
    // Offset trail pos so it comes out from the bottom of the spaceman's jet
    Point2D trail_off(30, 30);
    mid_bottom.setX(mid_bottom.getX() - trail_off.getX());
    mid_bottom.setY(mid_bottom.getY() - trail_off.getY());
    _base = (Point2D(mid_bottom.getX()*cos(rad_angle) - mid_bottom.getY()*sin(rad_angle), 
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

void Player::draw(AssetRenderer* _ass_rend, vector<Planet*> *g_objs) {
    // Render
    _ass_rend->render(getVerticeData(), 
                      getColourData(), 
                      getRotAngle(), 
                      GL_TRIANGLE_STRIP);

    // Update physics attributes only if box is moving
    if (_action == FLYING) {
        bool collision = PhysicsEngine::updatePhysics(*this, g_objs);
        _action = (collision) ? STILL : FLYING;
    }
}

void Player::applyGravity(vector<Planet*> *g_objs) {
    PhysicsEngine::applyGravityTo(*this, g_objs);
}

void Player::resetTime(float t) {
    vert_motion.setTime(t);
    hori_motion.setTime(t);
}

float Player::getRotAngle() const {
    return Math::normalizeAngle(_rot_angle - _rot_offset_angle, 0, 360);
}

float Player::getRotAngleOffset() const {
    return _rot_offset_angle;
}

float Player::getRealRotAngle() const {
    return _rot_angle;
}

void Player::setRotAngleOffset(float angle) {
    _rot_offset_angle = angle;
}

int Player::getOnPlanet() const {
    return _on_planet;
}

void Player::setOnPlanet(int index) {
    _on_planet = index;
}

float Player::getClosestPlanetDisp() const {
    return _closest_planet_disp;
}

void Player::setClosestPlanetDisp(float d) {
    _closest_planet_disp = d;
}
