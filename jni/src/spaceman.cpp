#include <stdlib.h>
#include "spaceman.h"
#include "game.h"
#include "point2d.h"
#include "log.h"
#include "physics.h"

Spaceman::Spaceman(float x, float y, Theme theme) : Player(x,y,50,100), _trail(27) {
    _action = FLYING;     // TODO use this after controls have been added -> _action = STILL;
    _facing = RIGHT;
    _frame = 0;           // TODO not implemented yet
    _colour_theme = theme;
    setRotAngle(0.0f);
}

Player::Action Spaceman::getAction() {
    return _action;
}

void Spaceman::setAction(Action act) {
    _action = act;
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

void Spaceman::drawTrail(ObjRenderer* _obj_rend, vector<Planet*> *g_objs) {
    // Render
    _trail.draw(_obj_rend, g_objs);
}

void Spaceman::update() {
    if (_action == FLYING) {
        _trail.buildTrail(_base.getX(), _base.getY(), getRotAngle(), _colour_theme);
    }
}

void Spaceman::applyGravity(vector<Planet*> *g_objs) {
    Player::applyGravity(g_objs);
    _trail.applyGravity(g_objs);
}

void Spaceman::resetTime() {
    Player::resetTime();
    _trail.resetTime();
}
