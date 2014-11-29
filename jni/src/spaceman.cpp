#include <stdlib.h>    // Needed for rand()
#include "motion.h"
#include "spaceman.h"

Spaceman::Spaceman(float x, float y) : Player(x,y,50,100) {
    // TODO use this after controls have been added -> _action = STILL;
    _action = FLYING;
    _facing = RIGHT;
    _frame = 0;
    _colour_theme = RAINBOW;
    rot_angle = 0.0f;
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
    render(time, screen_width, screen_height);
    _trail.render(_physics, time, screen_width, screen_height);
}

void Spaceman::update(float x, float y, float time) {
    setX(x - getWidth());
    setY(y - getHeight());
    if (_action == FLYING) {
        // Show the trail only if the player is flying
        _trail.createBox(x, y, time, _colour_theme, _physics);
    }
}

void Spaceman::setup(int screen_w, int screen_h) {
    renderer.setup(screen_w, screen_h);
    _trail.setup(screen_w, screen_h);
}

void Spaceman::render(int time, int screen_width, int screen_height) {
    // Render changes (Clear screen)
    renderer.renderFrame();

    // Draw every shape in shapes vector
    // Send player data to renderer
    renderer.renderObject(this);

    // Update physics attributes only if box is moving
    if (vert_motion.getVel() != 0.0f || hori_motion.getVel() != 0.0f)
        _physics.updatePhysics(*this, time, screen_width, screen_height);
}
