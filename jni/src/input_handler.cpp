#include "input_handler.h"
#include "collision.h"
#include "game.h"
#include "math.h"
#include "log.h"

#define OUT_touch_events false
#define OUT_nav_status false


InputHandler::InputHandler(bool nav_left_hand, Comp nav_axis, Player *user, Camera *cam) : _user(user), _cam(cam), _nav_axis(nav_axis), _nav_left_hand(nav_left_hand) { 
    _nav_active = false;
    _nav_touch_index = 0;
    _nav_starting_angle = 0.0f;
    _nav_starting_real_angle = 0.0f;
}

void InputHandler::restartValues(Point2D pos, int index) {
    _nav_starting_pt = Point2D(pos.getX(), pos.getY());
    _nav_touch_index = index;
    _nav_starting_angle = _user->getRotAngle();
    _nav_starting_real_angle = _user->getRealRotAngle();
    _nav_started_on_planets = _user->getOrbitingPlanets();
}

void InputHandler::touchDown(float x, float y, unsigned int index) {
    touchPointerDown(x, y, index);
}

void InputHandler::touchPointerDown(float x, float y, unsigned int index) {
    if (OUT_touch_events)
        LOGI("touch down %d", index);

    _touches.push_back(Touch(x, y));

    // Make sure touch pos was inside the navigation area
    Rect nav_rect = 
        (_nav_left_hand) ? Rect(0,                        0, Game::getScreenWidth()/2, Game::getScreenHeight()) :
                           Rect(Game::getScreenWidth()/2, 0, Game::getScreenWidth()/2, Game::getScreenHeight());

    if (Collision::isPtInRect(Point2D(x, y), nav_rect)) {
        if (!_nav_active) {
            restartValues(Point2D(x, y), index);
            _nav_active = true;
            if (OUT_nav_status)
                LOGI("nav active %i", _nav_touch_index);
        }
    }
}

void InputHandler::touchMove(float x, float y, unsigned int index) {
    if (OUT_touch_events)
        LOGI("touch move %d, x: %.2f, y: %.2f", index, x, y);

    _touches.at(index).pos = Point2D(x, y);

    if (_nav_active && (_nav_touch_index == index || _touches.size() == 1)) {
        vector<Planet*> now_orbiting = _user->getOrbitingPlanets();

        // Reset automatically when player goes in/out of a planet
        bool planets_changed = false;
        if (now_orbiting.size() != _nav_started_on_planets.size())
            planets_changed = true;
        else {
            for (int i=0; i<(int)now_orbiting.size(); i++) {
                if (now_orbiting.at(i) != _nav_started_on_planets.at(i)) {
                    planets_changed = true;
                    break;
                }
            }
        }

        if (planets_changed)
            restartValues(Point2D(x, y), index);

        float length = (_nav_axis == HORI) ? x - _nav_starting_pt.getX() : 
                                             y - _nav_starting_pt.getY();
        float nav_angle = _nav_starting_angle + (length/1.5);
        float change_in_angle = _nav_starting_real_angle - _user->getRealRotAngle();
        float net_angle = _user->getRealRotAngle() - nav_angle + change_in_angle;

        _user->setRotAngleOffset(net_angle);
    }
}

void InputHandler::touchUp(unsigned int index) {
    touchPointerUp(index);
}

void InputHandler::touchPointerUp(unsigned int index) {
    if (OUT_touch_events)
        LOGI("touch up %d", index);

    _touches.erase(_touches.begin() + index);

    if (_nav_active && (_nav_touch_index == index || _touches.size() == 0)) {
        _nav_active = false;
        if (OUT_nav_status)
            LOGI("nav not active %i", _nav_touch_index);
    }
}

void InputHandler::doubleTap(float x, float y) {
    LOGI("Double Tap");

    if (_user->getAction() == Action::STILL || _user->getAction() == Action::RUNNING) {
        _user->setAction(Action::FLYING);

        // Convert angle to anti-clockwise direction
        float angle = Math::normalizeAngle(180 - _user->getRotAngle(), 0, 360);

        // thrust up
        PhysicsEngine::genInitVel(*_user, angle, 30, 35, 0);
    }
}

Point2D InputHandler::getTouchPos(unsigned int index) const {
    return _touches.at(index).pos;
}

unsigned int InputHandler::getCount() const {
    return _touches.size();
}

bool InputHandler::isNavActive() const {
    return _nav_active;
}
