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
    _frame = 0;           // TODO not implemented yet
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
    vector<float> tex_vert;

    // flip texture horizontally
    if (_facing == LEFT) {
        float tex_left_vert[] = {
            1.0, 1.0,
            1.0, 0.0,
            0.0, 1.0,
            0.0, 0.0
        };

        tex_vert = vector<float> (tex_left_vert, tex_left_vert + sizeof(tex_left_vert) / sizeof(float));
    }
    else {
        float tex_default_vert[] = {
            0.0, 1.0,
            0.0, 0.0,
            1.0, 1.0,
            1.0, 0.0
        };

        tex_vert = vector<float> (tex_default_vert, tex_default_vert + sizeof(tex_default_vert) / sizeof(float));
    }

    // Render
    rend->render(getVerticeData(),
                 Colour::getColour(_colour_theme),
                 tex_vert,
                 tex->getTex(TEX_SPACEMAN), 
                 getRotAngle(),
                 _facing, 
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
        if (_action != Action::FLYING) _action = Action::FLYING;
    }
    // else {
        // Convert angle to anti-clockwise direction
        float angle = Math::normalizeAngle(180 - getRotAngle(), 0, 360);

        // thrust up
        PhysicsEngine::genInitVel(*this, angle, 1, 1.5, 0);
        resetTime(Game::getElapsedTime());
        _trail.buildTrail(_base.getX(), 
                _base.getY(), 
                360 - getRotAngle(), 
                _colour_theme);
    // }
}

void Spaceman::applyGravity(vector<Planet*> *g_objs) {
    Player::applyGravity(g_objs);
    _trail.applyGravity(g_objs);
}

void Spaceman::resetTime(float t) {
    Player::resetTime(t);
    _trail.resetTime(t);
}
