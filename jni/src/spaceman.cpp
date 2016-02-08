#include <stdlib.h>
#include <algorithm>
#include "spaceman.h"
#include "game.h"
#include "point2d.h"
#include "log.h"
#include "physics.h"
#include "math.h"

Spaceman::Spaceman(float x, float y, Theme theme) : Player(x,y,105,125.3), _trail(22) {
    _action = Action::FLYING;
    _frame = 25;
    _colour_theme = theme;
    setRotAngle(0.0f);
}

Dir Spaceman::getFacing() {
    return _facing;
}

void Spaceman::setFacing(Dir dir) {
    _facing = dir;
}

int Spaceman::getFrame() {
    return _frame;
}

void Spaceman::setFrame(int frame) {
    _frame = frame;
}

void Spaceman::changeTheme(Theme &old_theme) {
    Theme new_theme(GRAY);
    do {
        int rand_val = rand() % 250;
        if (rand_val >= 0 && rand_val < 50)
            new_theme = GRAY;
        else if (rand_val >= 50  && rand_val < 100)
            new_theme = RED;
        else if (rand_val >= 100 && rand_val < 150)
            new_theme = BLUE;
        else if (rand_val >= 150 && rand_val < 200)
            new_theme = PURPLE;
        else if (rand_val >= 200 && rand_val < 250)
            new_theme = RAINBOW;
    } while (new_theme == 0 || new_theme == old_theme);

    old_theme = new_theme;
}

void Spaceman::draw(PlayerRenderer* rend, vector<Planet*> *g_objs, TextureHandler *tex) {
    // Render
    rend->render(getVerticeData(),
                 Colour::getColour(_colour_theme),
                 TextureHandler::calcTexVerticesFromSpritesheet(1925, 2760, 5, 6, _frame, (_facing != RIGHT)),
                 tex->getTex(TEX_SPACEMAN), 
                 getRotAngle(),
                 GL_TRIANGLE_STRIP);

    Player::draw(rend, g_objs, tex);
}

void Spaceman::drawTrail(ObjRenderer* rend, vector<Planet*> *g_objs) {
    // Render
    _trail.draw(rend, g_objs);
}

void Spaceman::update() {
    if (_action == Action::STILL) {
        // TODO add running code here
        // PhysicsEngine::genInitVel(*this, getRotAngle()+90, 1, 1.5, 0);
        // hori_motion.setVel(hori_motion.getVel() + 1.0f);
        // if (_action != Action::FLYING) 
        // _action = Action::FLYING;

        _action = Action::RUNNING;
    }
    else if (_action == Action::RUNNING) {
        if (_on_planet_region == -1) {
            float speed = (_on_planet->getWidth()/400) * ((_facing == RIGHT) ? _running_speed : -_running_speed);
            Point2D pt = Math::rotatePtAroundPt(_on_planet->getCentre(), getCentre(), speed);

            // Point2D offset_ctr = Point2D(fabs(getCentreX() - _on_planet->getCentreX()) - _on_planet->getWidth(),
            //                              fabs(getCentreY() - _on_planet->getCentreY()) - _on_planet->getHeight());
            //
            // setX(pt.getX() - ((getWidth() + offset_ctr.getX())/2));
            // setY(pt.getY() - ((getHeight() + offset_ctr.getY())/2));
            setX(pt.getX() - (getWidth()/2));
            setY(pt.getY() - (getHeight()/2));
        }
        else {
            if (_facing == RIGHT) {
                setX(getX() + (_running_speed*-_running_unit_vector.getY()));
                setY(getY() + (_running_speed*_running_unit_vector.getX()));
            }
            else {
                setX(getX() + (_running_speed*_running_unit_vector.getY()));
                setY(getY() + (_running_speed*-_running_unit_vector.getX()));
            }
        }


        // TODO will need to add an offset to rep the disp from the centre
        // aswell as a func that realigns the player to the normal

        // Point2D offset_ctr = Point2D((getCentreX() - _on_planet->getCentreX()),
        //                              (getCentreY() - _on_planet->getCentreY()));

        // Convert angle to anti-clockwise direction
        // float angle = Math::normalizeAngle(180 - getRotAngle(), 0, 360);
        //
        // // thrust up
        // PhysicsEngine::genInitVel(*this, angle, 1, 1.5, 0);
        // resetTime(Game::getElapsedTime());
        // _trail.buildTrail(_base.getX(), 
        //                   _base.getY(), 
        //                   360 - getRotAngle(), 
        //                   _colour_theme);
        //
        //
        // float disp = Math::getHypotenuse(fabs(getCentreX() - _on_planet->getCentreX()),
        //                                  fabs(getCentreY() - _on_planet->getCentreY()));
        //
        // // _width_offset = (_on_planet->getWidth()/2) - offset_ctr.getX();
        // // _height_offset = (_on_planet->getHeight()/2) - offset_ctr.getY();
        // LOGI("disp %.3f", disp);

        // float x = getWidth() - offset_ctr.getX();
        // float y = getHeight() - offset_ctr.getY();
        // if (x > )

        // setX(getX() + x);
        // setY(getY() + y);

        // LOGI("(%.2f, %.2f), %.2f", offset_ctr.getX(), offset_ctr.getY(), _closest_planet_disp);
        //
        // setX(pt.getX() - offset_ctr.getX());
        // setY(pt.getY() - offset_ctr.getY());

        // Convert angle to anti-clockwise direction
        // float angle = Math::normalizeAngle(180 - getRotAngle(), 0, 360);
        // vector<Planet*> g_objs = getOrbitingPlanets();

        // thrust up
        // PhysicsEngine::genInitVel(*this, angle, 1.2, 1.2, 0);
        // PhysicsEngine::genInitVel(*this, angle + ((_facing == RIGHT) ? -90 : 90), 1.5, 1.5, 0);
        // PhysicsEngine::updatePhysics(*this, &g_objs);
        // resetTime(Game::getElapsedTime());

        // float x = 0;
        // float y = 0;
        // PhysicsEngine::splitValueFromAngle(_running_speed, getRotAngle()+90, &x, &y);
        //
        // setX(getX() - x);
        // setY(getY() + y);
        

        // vector<float> vertices = getVerticeData();

        

    }
    
    if (_action == Action::FLYING || _action == Action::LANDING){
        // Convert angle to anti-clockwise direction
        float angle = Math::normalizeAngle(180 - getRotAngle(), 0, 360);

        // thrust up
        PhysicsEngine::genInitVel(*this, angle, 1, 1.5, 0);
        resetTime(Game::getElapsedTime());
        _trail.buildTrail(_base.getX(), 
                          _base.getY(), 
                          360 - getRotAngle(), 
                          _colour_theme);
    }


    // animate
    if (_frame < 29)
        _frame++;
    else
        _frame = 0;

        // float disp = Math::getHypotenuse(fabs(getCentreX() - _on_planet->getCentreX()),
                                         // fabs(getCentreY() - _on_planet->getCentreY()));

        // _width_offset = (_on_planet->getWidth()/2) - offset_ctr.getX();
        // _height_offset = (_on_planet->getHeight()/2) - offset_ctr.getY();
        // LOGI("disp %.3f", disp);
        // LOGI("(%.2f, %.2f)", getCentreX(), getCentreY());

    // LOGI("action %s", Action::toString(_action).c_str());
}

void Spaceman::applyGravity(vector<Planet*> *g_objs) {
    // if (_action != Action::RUNNING) {
        Player::applyGravity(g_objs);
        _trail.applyGravity(g_objs);
    // }
    // else {
    //     Planet *plan = getOnPlanet();
    //
    //     // Create rect to rep the planets gravity area
    //     float g_radius = plan->getWidth();
    //     Rect grav_rect(plan->getX() - g_radius,
    //                    plan->getY() - g_radius,
    //                    plan->getWidth() + (g_radius*2),
    //                    plan->getWidth() + (g_radius*2));
    //
    //     float angle_new = PhysicsEngine::getAngleOfPtFromRectCentre(getCentre(), grav_rect);
    //     setRotAngle(-angle_new);
    // }
}

void Spaceman::resetTime(float t) {
    Player::resetTime(t);
    _trail.resetTime(t);
}
