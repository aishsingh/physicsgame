#include <stdlib.h>    // Needed for rand()
#include "spaceman.h"
#include "game.h"
#include "point2d.h"
#include "log.h"

Spaceman::Spaceman(float x, float y, Theme theme, Rend_player* pla_rend, Rend_box* trail_rend) : Player(x,y,50,100,pla_rend), _trail(27,trail_rend) {
    _action = Action::FLYING;     // TODO use this after controls have been added -> _action = STILL;
    _facing = Dir::RIGHT;
    _frame = 0;           // TODO not implemented yet
    _colour_theme = theme;
    setRotAngle(-360.0f); // TODO should work with 0.0f
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
    Theme new_theme = (Theme) 0;
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

void Spaceman::draw() {
    // Render
    _rend->renderPlayer(this);

    // Update physics attributes only if box is moving
    // if (vert_motion.getVel() != 0.0f || hori_motion.getVel() != 0.0f)
    //     _physics.updatePhysics(*this, Game::getElapsedTime(), Game::getScreenWidth(), Game::getScreenHeight());

    // Attributes need to be disabled to avoid different shaders from reading in random values
    _rend->disableAttributes();
}
void Spaceman::drawTrail() {
    // Render
    _trail.render(_physics);
}

void Spaceman::update(float x, float y, float angle, bool build_trail) {
    // Update player attributes
    // setX(x - getWidth());
    // setY(y - getHeight());
    
    // Rotate player from joystick
    setRotAngle(angle);

    if (build_trail) {
        if (_action == FLYING) {
            _trail.buildTrail(_base.getX(), _base.getY(), angle, _colour_theme, _physics);
        }
    }
}
