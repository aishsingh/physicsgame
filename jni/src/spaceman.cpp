#include <stdlib.h>
#include <algorithm>
#include "spaceman.h"
#include "config.h"
#include "game.h"
#include "point2d.h"
#include "physics.h"
#include "math.h"
#include "log.h"

Spaceman::Spaceman(float x, float y, Theme theme) : Player(x,y,105,125.3,theme) {
    _action = Action::FLYING;
    _frame = 25;
}

void Spaceman::draw(PlayerRenderer* rend, TextureHandler *tex) {
    // Render
    rend->render(getVerticeData(),
                 Colour::getColour(_colour_theme),
                 TextureHandler::calcTexVerticesFromSpritesheet(1925, 2760, 5, 6, _frame, (_facing != RIGHT)),
                 tex->getTex(TEX_SPACEMAN), 
                 getRotAngle(),
                 GL_TRIANGLE_STRIP);
}

void Spaceman::updateAction(const vector<GravObject*> *g_objs) {
    switch (_action) {
        case Action::STILL: {
            _action = Action::RUNNING;
            break;
        }

        case Action::RUNNING: {
            if (_on_planet_region == -1) {
                // FIXME
                // float speed = ((_facing == RIGHT) ? _running_speed : -_running_speed)/((_on_planet->getWidth()/150)*2);
                // Point2D pt = Math::rotatePtAroundPt(_on_planet->getCentre(), getCentre(), speed);
                //
                // setX(pt.getX() - (getWidth()/2));
                // setY(pt.getY() - (getHeight()/2));

            }
            else {
                if (_facing == RIGHT) {
                    setX(getX() + (_running_speed*_running_unit_vector.getX()));
                    setY(getY() + (_running_speed*_running_unit_vector.getY()));
                }
                else {
                    setX(getX() + (_running_speed*-_running_unit_vector.getX()));
                    setY(getY() + (_running_speed*-_running_unit_vector.getY()));
                }
            }
            break;
        }

        case Action::FLYING:
        case Action::LANDING: {
            // Convert angle to anti-clockwise direction
            float angle = Math::normalizeAngle(180 - getRotAngle(), 0, 360);

            // Thrust up
            Physics::genInitVel(*this, angle, PLAYER_THRUST_MIN_INIT_V, PLAYER_THRUST_MAX_INIT_V, PLAYER_THRUST_OFFSET);
            resetTime(Game::getElapsedTime());
            _trail.buildTrail(_base.getX(), 
                              _base.getY(), 
                              360 - getRotAngle(), 
                              _colour_theme);
            break;
        }
    }

    // Animate
    if (_frame < 29)
        _frame++;
    else
        _frame = 0;
}
