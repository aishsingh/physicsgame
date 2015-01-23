#include <stdlib.h>
#include "spaceman.h"
#include "game.h"
#include "point2d.h"
#include "log.h"
#include "physics.h"
#include "math.h"

Spaceman::Spaceman(float x, float y, Theme theme) : Player(x,y,105,125.3), _trail(22) {
    _action = FLYING;
    _facing = RIGHT;
    _frame = 0;           // TODO not implemented yet
    _colour_theme = theme;
    setRotAngle(0.0f);
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

void Spaceman::draw(AssetRenderer* _ass_rend, vector<Planet*> *g_objs, TextureHandler *tex) {
    float tex_vert[] = {
        0.0, 1.0,
        0.0, 0.0,
        1.0, 1.0,
        1.0, 0.0
    };

    // Render
    _ass_rend->render(getVerticeData(),
                      vector<float> (tex_vert, tex_vert + sizeof(tex_vert) / sizeof(float)),
                      tex->getTex(TEX_SPACEMAN), 
                      getRotAngle(), 
                      GL_TRIANGLE_STRIP);

    Player::draw(_ass_rend, g_objs, tex);
}

void Spaceman::drawTrail(ObjRenderer* _obj_rend, vector<Planet*> *g_objs) {
    // Render
    _trail.draw(_obj_rend, g_objs);
}

void Spaceman::update() {
    switch (_action) {
        case STILL: {
            // Convert angle to anti-clockwise direction
            float angle = Math::normalizeAngle(180 - getRotAngle(), 0, 360);

            PhysicsEngine::genInitVel(*this, angle, 2, 1, 0);
            resetTime(Game::getElapsedTime());
            _trail.buildTrail(_base.getX(), 
                    _base.getY(), 
                    360 - getRotAngle(), 
                    _colour_theme);
            _action = FLYING;
            break;
        }

        case FLYING: {
            // Convert angle to anti-clockwise direction
            float angle = Math::normalizeAngle(180 - getRotAngle(), 0, 360);

            PhysicsEngine::genInitVel(*this, angle, 2, 1, 0);
            _trail.buildTrail(_base.getX(), 
                    _base.getY(), 
                    360 - getRotAngle(), 
                    _colour_theme);
            break;
        }

        case RUNNING: {
            // TODO
            break;
        }
    }
}

void Spaceman::applyGravity(vector<Planet*> *g_objs) {
    Player::applyGravity(g_objs);
    _trail.applyGravity(g_objs);
}

void Spaceman::resetTime(float t) {
    Player::resetTime(t);
    _trail.resetTime(t);
}
