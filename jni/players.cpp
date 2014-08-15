#include "motion.h"
#include "players.h"

Spaceman::Spaceman(const float x, const float y, const float width, const float height) : Object(x,y,width,height) {
    _action = STILL;
    _facing = RIGHT;
    _frame = 0;
}

Action Spaceman::getAction() {
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
