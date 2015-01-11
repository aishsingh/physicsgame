#include "input_handler.h"
#include "collision.h"
#include "game.h"

InputHandler::InputHandler(bool nav_left_hand, Comp nav_axis, Player *user) : _user(user), _nav_axis(nav_axis), _nav_left_hand(nav_left_hand) { 
    _nav_active = false;
    _nav_starting_angle = 0.0f;
    _nav_touch_index = 0;
}

void InputHandler::touchDown(float x, float y, unsigned int index) {
    _touches.push_back(Touch(x, y, index));

    // Make sure touch pos was inside the navigation area
    Rect nav_rect = 
        (_nav_left_hand) ? Rect(0,                        0, Game::getScreenWidth()/2, Game::getScreenHeight()) :
                           Rect(Game::getScreenWidth()/2, 0, Game::getScreenWidth()/2, Game::getScreenHeight());

    if (Collision::isPtInRect(Point2D(x, y), nav_rect)) {
        if (!_nav_active) {
            _nav_starting_pt = Point2D(x, y);
            _nav_starting_angle = _user->getRotAngle();
            _nav_touch_index = index;
            _nav_active = true;
        }
    }
}

void InputHandler::touchPointerDown(float x, float y, unsigned int index) {
    touchDown(x, y, index);
}

void InputHandler::touchMove(float x, float y, unsigned int index) {
    _touches.at(index).pos = Point2D(x, y);

    if (_nav_active && _nav_touch_index == index) {
        float length = (_nav_axis == HORI) ? x - _nav_starting_pt.getX() : 
                                             y - _nav_starting_pt.getY();
        float angle = _nav_starting_angle + length;
        _user->setRotAngle(angle);
    }
}

void InputHandler::touchUp(unsigned int index) {
    _touches.erase(_touches.begin() + index);

    if (_nav_touch_index == index) {
        _nav_active = false;
    }
}

void InputHandler::touchPointerUp(unsigned int index) {
    touchUp(index);
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
