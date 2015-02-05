#include <math.h>
#include "player.h"
#include "physics.h"
#include "math.h"
#include "texture_handler.h"

Player::Player(float x, float y, float width, float height) : Object(x,y,width,height) {
    _rot_offset_angle = 0.0f;
    _on_planet = 0;
    _on_planets_count = 0;
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

    /* Set base value (bottom of the player after rotation)
     * These calculations ensure that the base pos is the same as it would be after it went through the vshader.
     * The vshader rotates the pos, so the same rotation is applyed here */
    Point2D mid_bottom((vec[2] + vec[6])/2, (vec[3] + vec[7])/2);
    float rad_angle = getRotAngle()*PI/180.0f;
    // Offset trail pos so it comes out from the bottom of the spaceman's jet
    Point2D trail_off(30, 30);
    mid_bottom.setX(mid_bottom.getX() - trail_off.getX());
    mid_bottom.setY(mid_bottom.getY() - trail_off.getY());
    _base = (Point2D(mid_bottom.getX()*cos(rad_angle) - mid_bottom.getY()*sin(rad_angle), 
                     mid_bottom.getY()*cos(rad_angle) + mid_bottom.getX()*sin(rad_angle)));

    return std::vector<float> (vec, vec + sizeof(vec) / sizeof(float));
}

void Player::draw(PlayerRenderer* rend, vector<Planet*> *g_objs, TextureHandler *tex) {
    // Update physics attributes only if box is moving
    if (_action == FLYING || _action == LANDING) {
        bool is_landed = PhysicsEngine::updatePhysics(*this, g_objs);
        if (is_landed)
            _action = STILL;
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
    _rot_offset_angle = Math::normalizeAngle(angle, 0, 180);
}

int Player::getOnPlanet() const {
    return _on_planet;
}

void Player::setOnPlanet(int index) {
    _on_planet = index;
}
unsigned Player::getOnPlanetsCount() const {
    return _on_planets_count;
}

void Player::setOnPlanetsCount(unsigned count) {
    _on_planets_count = count;
}

float Player::getClosestPlanetDisp() const {
    return _closest_planet_disp;
}

void Player::setClosestPlanetDisp(float d) {
    _closest_planet_disp = d;
}

Action Player::getAction() {
    return _action;
}

void Player::setAction(Action act) {
    _action = act;
}

