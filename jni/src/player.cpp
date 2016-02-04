#include <math.h>
#include "player.h"
#include "physics.h"
#include "math.h"
#include "texture_handler.h"
#include "log.h"

Player::Player(float x, float y, float width, float height) : Object(x,y,width,height) {
    _rot_offset_angle = 0.0f;
    _on_planet = NULL;
    _last_visited_planet = NULL;
    _facing = LEFT;
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
    Point2D trail_off;
    if (_facing == LEFT)
        trail_off = Point2D(-30, 30);
    else
        trail_off = Point2D(30, 30);

    mid_bottom.setX(mid_bottom.getX() - trail_off.getX());
    mid_bottom.setY(mid_bottom.getY() - trail_off.getY());
    _base = Point2D(mid_bottom.getX()*cos(rad_angle) - mid_bottom.getY()*sin(rad_angle), 
                    mid_bottom.getY()*cos(rad_angle) + mid_bottom.getX()*sin(rad_angle));

    return std::vector<float> (vec, vec + sizeof(vec) / sizeof(float));
}

void Player::draw(PlayerRenderer* rend, vector<Planet*> *g_objs, TextureHandler *tex) {
    // Update physics attributes only if box is moving
    if (_action == Action::FLYING || _action == Action::LANDING) {
        PhysicsEngine::updatePlayerOrbittingPlanets(*this, g_objs);

        Planet* on_g_obj = PhysicsEngine::updatePhysics(*this, g_objs);
        // if there actually was a collision
        _on_planet = on_g_obj;
        if (on_g_obj != NULL) {
            _action = Action::STILL;
            _orbiting_planets.clear(); // only keep track of the planet the player is on
            _orbiting_planets.push_back(_on_planet); // only keep track of the planet the player is on
            _last_visited_planet = _on_planet;
        }
    }
    else if (_action == Action::STILL) {
        _on_planet->anchorObject(this);
    }
}

void Player::drawStats(ObjRenderer* rend, vector<Planet*> *g_objs) {
    // display actions
    // LOGI("%s", Action::toString(_action).c_str());

    // closest planet tracking
    vector<float> closest_planet;
    for (int i=0; i<(int)_orbiting_planets.size(); i++) {
        Planet *p = _orbiting_planets.at(i);
        closest_planet.push_back(getCentreX());
        closest_planet.push_back(getCentreY());
        closest_planet.push_back(p->getCentreX());
        closest_planet.push_back(p->getCentreY());
    }

    rend->render(closest_planet,
                 Colour::getColour(RED),
                 0.0f,
                 GL_LINES);
}

void Player::applyGravity(vector<Planet*> *g_objs) {
    PhysicsEngine::applyGravityTo(*this, &_orbiting_planets);
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

Planet* Player::getOnPlanet() const {
    return _on_planet;
}

void Player::setOnPlanet(Planet* p) {
    _on_planet = p;
}

int Player::getOrbitingPlanetsCount() const {
    return _orbiting_planets.size();
}

vector<Planet*> Player::getOrbitingPlanets() const {
    return _orbiting_planets;
}

void Player::setOrbitingPlanets(vector<Planet*> p) {
    _orbiting_planets = p;   
}

float Player::getClosestPlanetDisp() const {
    return _closest_planet_disp;
}

void Player::setClosestPlanetDisp(float d) {
    _closest_planet_disp = d;
}

Action::Action Player::getAction() {
    return _action;
}

void Player::setAction(Action::Action act) {
    _action = act;
}

void Player::updateDir() {
    if (_orbiting_planets.size() == 1) {
        float nav_angle = getRotAngleOffset();
        if (nav_angle > 45.0f && nav_angle < 90.0f)
            _facing = LEFT;
        else if (nav_angle < -45.0f && nav_angle > -90.0f)
            _facing = RIGHT;
    }
}
