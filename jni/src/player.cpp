#include <math.h>
#include "player.h"
#include "config.h"
#include "physics.h"
#include "math.h"
#include "texture_handler.h"
#include "game.h"
#include "log.h"

Player::Player(float x, float y, float width, float height) : Object(x,y,width,height) {
    _rot_offset_angle = 0.0f;
    _running_speed = 9.0f;
    _on_planet = NULL;
    _last_visited_planet = NULL;
    _closest_planet_disp = 0;
    _facing = LEFT;
    _on_planet_region = -1;
    _on_planet_region_prev = -1;
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

void Player::draw(PlayerRenderer* rend, vector<GravObject*> *g_objs, TextureHandler *tex) {
    // Update physics attributes only if box is moving
    if (_action == Action::FLYING || _action == Action::LANDING) {
        PhysicsEngine::updatePlayerOrbittingPlanets(*this, g_objs);

        _on_planet = PhysicsEngine::updatePhysicsForCollisions(this, g_objs);

        // if there actually was a collision
        if (_on_planet != NULL) {
            _action = Action::STILL;
            _orbiting_planets.clear(); // only keep track of the planet the player is on
            _orbiting_planets.push_back(_on_planet); // only keep track of the planet the player is on
            _last_visited_planet = _on_planet;

            CollisionData c = CollisionData(_on_planet);
            Collision::genCollisionData(*this, &c, _facing);
            _on_planet_region = c.region;
            _running_unit_vector = c.unit_vec;

            if (OUT_PLAYER_SURFACE_OFFSET)
                LOGI("Surface offset: (%.2f)", c.offset);
        }
    }
    else if (_action == Action::RUNNING) {
        // rotate player along with the planet's rotation
        _on_planet->anchorObject(this);

        // physics funcs only need the current planet
        vector<GravObject*> p; p.push_back(_on_planet);

        PhysicsEngine::updatePhysicsForCollisions(this, &p);
        CollisionData c = CollisionData(_on_planet);
        Collision::genCollisionData(*this, 
                                    &c, 
                                    _facing, 
                                    _on_planet_region, 
                                    Point2D(getRunningUnitVector().getY(), -getRunningUnitVector().getX()));

        // apply collision data if on a valid region
        if (c.region != -1) {
            if (c.region != _on_planet_region) {
                if (c.region != _on_planet_region_prev) {
                    _on_planet_region_prev = _on_planet_region;
                    _on_planet_region = c.region;
                    _running_unit_vector = c.unit_vec;
                }
            }
            _on_planet_collision_pt = c.pt;
        }
        else {
            // Point2D pt = Math::rotateObj(this);
            // float x = pt.getX();
            // float y = pt.getY();
            //
            // Point2D A = Point2D(x, y);
            // Point2D B = Point2D(x, y + getHeight());
            // _running_unit_vector = Math::getUnitVector(Math::getNormal(A, B));

            // Point2D unit_vec = Point2D(getRunningUnitVector().getY(), -getRunningUnitVector().getX());
            // _on_planet_collision_pt = getCentre() - (unit_vec*(getHeight()/2));
        }

        if (OUT_PLAYER_SURFACE_OFFSET)
            LOGI("Surface offset: (%.2f)", c.offset);
    }
    else {
        // rotate player along with the planet's rotation
        _on_planet->anchorObject(this);
        _running_unit_vector = Math::rotatePt(_running_unit_vector, -_on_planet->getRotSpeed());
        // _on_planet_collision_pt = Math::rotatePt(_on_planet_collision_pt, -_on_planet->getRotSpeed());

        float t = Game::getElapsedTime();
        hori_motion.setTime(t);
        vert_motion.setTime(t);
    }

    // display actions
    if (OUT_PLAYER_ACTION)
        LOGI("%s", Action::toString(_action).c_str());
}

void Player::drawStats(ObjRenderer* rend) {

    // closest planet tracking
    if (STATS_PLAYER_CLOSEST_GOBJ) {
        vector<float> closest_planet;
        for (int i=0; i<(int)_orbiting_planets.size(); i++) {
            GravObject *p = _orbiting_planets.at(i);
            closest_planet.push_back(getCentreX());
            closest_planet.push_back(getCentreY());
            closest_planet.push_back(p->getCentreX());
            closest_planet.push_back(p->getCentreY());
        }

        rend->render(closest_planet,
                     STATS_COLOUR,
                     0.0f,
                     GL_LINES);
    }

    // arrow representing the running direction of the player
    if (STATS_PLAYER_RUNNING_DIR && (_action == Action::RUNNING || _action == Action::STILL)) {
        vector<float> running_dir;
        float arrow_length = 80.0f;
        float arrow_off = 8.0f;

        running_dir.push_back(getCentreX());
        running_dir.push_back(getCentreY());
        if (_facing == RIGHT) {
            running_dir.push_back(getCentreX() + (arrow_length*_running_unit_vector.getX()));
            running_dir.push_back(getCentreY() + (arrow_length*_running_unit_vector.getY()));

            running_dir.push_back(running_dir.at(2));
            running_dir.push_back(running_dir.at(3));
            running_dir.push_back(running_dir.at(2) + (-arrow_off*_running_unit_vector.getX()) + (arrow_off*_running_unit_vector.getY()));
            running_dir.push_back(running_dir.at(3) + (-arrow_off*_running_unit_vector.getY()) + (-arrow_off*_running_unit_vector.getX()));

            running_dir.push_back(running_dir.at(2));
            running_dir.push_back(running_dir.at(3));
            running_dir.push_back(running_dir.at(2) + (-arrow_off*_running_unit_vector.getX()) + (-arrow_off*_running_unit_vector.getY()));
            running_dir.push_back(running_dir.at(3) + (-arrow_off*_running_unit_vector.getY()) + (arrow_off*_running_unit_vector.getX()));
        }
        else {
            running_dir.push_back(getCentreX() + (arrow_length*-_running_unit_vector.getX()));
            running_dir.push_back(getCentreY() + (arrow_length*-_running_unit_vector.getY()));

            running_dir.push_back(running_dir.at(2));
            running_dir.push_back(running_dir.at(3));
            running_dir.push_back(running_dir.at(2) + (arrow_off*_running_unit_vector.getX()) + (arrow_off*_running_unit_vector.getY()));
            running_dir.push_back(running_dir.at(3) + (arrow_off*_running_unit_vector.getY()) + (-arrow_off*_running_unit_vector.getX()));

            running_dir.push_back(running_dir.at(2));
            running_dir.push_back(running_dir.at(3));
            running_dir.push_back(running_dir.at(2) + (arrow_off*_running_unit_vector.getX()) + (-arrow_off*_running_unit_vector.getY()));
            running_dir.push_back(running_dir.at(3) + (arrow_off*_running_unit_vector.getY()) + (arrow_off*_running_unit_vector.getX()));
        }

        rend->render(running_dir,
                     STATS_COLOUR,
                     0.0f,
                     GL_LINES);
    }

    // Point where the current planet uses to find the region its on
    if (STATS_PLAYER_REGION_PT && _on_planet) {
        vector<float> cross;
        float off = 50;

        // Point2D unit_vec = Point2D(getRunningUnitVector().getY(), -getRunningUnitVector().getX());
        Point2D pt = _on_planet_collision_pt;//getCentre() - (unit_vec*(getHeight()/2));
        cross.push_back(pt.getX() + off);
        cross.push_back(pt.getY() + off);
        cross.push_back(pt.getX() - off);
        cross.push_back(pt.getY() - off);

        cross.push_back(pt.getX() - off);
        cross.push_back(pt.getY() + off);
        cross.push_back(pt.getX() + off);
        cross.push_back(pt.getY() - off);

        rend->render(cross,
                     STATS_COLOUR,
                     0.0f,
                     GL_LINES);
    }
}

void Player::applyGravity() {
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

GravObject* Player::getOnPlanet() const {
    return _on_planet;
}

void Player::setOnPlanet(GravObject* p) {
    _on_planet = p;
}

int Player::getOnPlanetRegion() const {
    return _on_planet_region;
}

Point2D Player::getRunningUnitVector() const {
    return _running_unit_vector;
}

int Player::getOrbitingPlanetsCount() const {
    return _orbiting_planets.size();
}

vector<GravObject*> Player::getOrbitingPlanets() const {
    return _orbiting_planets;
}

void Player::setOrbitingPlanets(vector<GravObject*> p) {
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
