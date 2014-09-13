#include <stdlib.h>    // Needed for rand()
#include "motion.h"
#include "players.h"

Spaceman::Spaceman(const float x, const float y, const float width, const float height) : Object(x,y,width,height) {
    // TODO use this after controls have been added -> _action = STILL;
    _action = FLYING;
    _facing = RIGHT;
    _frame = 0;
    _colour_theme = RAINBOW;
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

void Spaceman::draw(int time, int screen_width, int screen_height) {
    trail.render(_physics, time, screen_width, screen_height);
}

void Spaceman::update(float x, float y, float time) {
    if (_action == FLYING) {
        // Show the trail only if the player is flying
        trail.createBox(x, y, time, _colour_theme, _physics);
    }
}

void Spaceman::setup(int screen_w, int screen_h) {
    trail.setup(screen_w, screen_h);
}
