#include <stdlib.h>                // Needed for rand()
#include <math.h>                  // Needed for math functions

#include "physics.h"
#include "log.h"

PhysicsEngine::PhysicsEngine() {
    /* Initialize */
    _MAX_INITIAL_VERT_VELOCITY = -25.0f;
    _MIN_INITIAL_VERT_VELOCITY = -20.0f;
    _MAX_INITIAL_HORI_VELOCITY = 4.0f;
    _MAX_G_SWITCH_MISCALC = 3.0f;
    _gravity = 0.98f; //2.5f; // normal = 0.75f;
}

float PhysicsEngine::getGravity() {
    return _gravity;
}

Motion PhysicsEngine::calcMotion(Object &obj, const Comp comp, const float &elapsed_time) {
    Motion calc;

    if (comp == VERT) {
        ///-----------------------------------------------------------------------------
        // Physics equations of constant acceleration used to find new displacement
        // d = (final velocity * change in time) - (1/2 * acceleration * current time^2)


        float tFinal = elapsed_time;
        float ct = tFinal - obj.vert_motion.getTime();
        float vFinal = obj.vert_motion.getVel() + (obj.vert_motion.getAccel() * ct);
        float d = (vFinal * ct) - (0.5 * obj.vert_motion.getAccel() * pow(ct, 2.0));
        // SDL_Log("tFinal:%.4f, ct:%.4f, vFinal:%.4f, d:%.4f", tFinal, ct, vFinal, d);

        ///-----------------------------------------------------------------------------

        calc.setChangeTime(ct);
        calc.setDisp(d);
        calc.setVel(vFinal);
        calc.setTime(tFinal);
    }
    else if (comp == HORI) {
        ///-----------------------------------------------------------------------------

        // float tFinal = elapsed_time;
        // float ct = tFinal - obj.hori_motion.getTime();
        // float vFinal = obj.hori_motion.getVel();
        // float d = (vFinal * ct);
        float tFinal = elapsed_time;
        float ct = tFinal - obj.hori_motion.getTime();
        float vFinal = obj.hori_motion.getVel() + (obj.hori_motion.getAccel() * ct);
        float d = (vFinal * ct) - (0.5 * obj.hori_motion.getAccel() * pow(ct, 2.0));
        // SDL_Log("tFinal:%.4f, ct:%.4f, vFinal:%.4f, d:%.4f", tFinal, ct, vFinal, d);

        ///-----------------------------------------------------------------------------
        if (obj.vert_motion.getAccel() == 0) {
            // if (obj.hori_motion.getVel() > 0)
            //     calc.setAccel(1);
            // else
            //     calc.setAccel(-1);

            calc.setChangeTime(ct);
            calc.setDisp(d);
            calc.setTime(tFinal);
        } else {

            calc.setChangeTime(ct);
            calc.setDisp(d);
            calc.setVel(vFinal);
            calc.setTime(tFinal);
        }
    }
    else if (comp == CIRC) {
        // Circular motion
        float offset = 1.5f;
        obj.rot_angle += obj.hori_motion.getVel() * offset;

        // LOGI("angle [%.2f]", obj.rot_angle);
    }

    return calc;
}

void PhysicsEngine::updatePhysics(Object &obj, const float &elapsed_time, const int &screen_width, const int &screen_height) {
    int origin_x = 0;//screen_width/-1;  // usually 0
    int origin_y = 0;//screen_height/-1; // usually 0
    Motion vertComp = calcMotion(obj, VERT, elapsed_time);
    Motion horiComp = calcMotion(obj, HORI, elapsed_time);
    Motion circComp = calcMotion(obj, CIRC, elapsed_time);

    // Vertical Component
    if (obj.getY() + vertComp.getDisp() + obj.getHeight() > screen_height) {    // Check if not too high
        obj.setY(screen_height - obj.getHeight());
        obj.vert_motion.setVel(0.0f);
    } 
    else if (obj.getY() + vertComp.getDisp() < origin_y) {                      // Check if not too low
        obj.setY(origin_y); // usually 0
        obj.vert_motion.setVel(0.0f);
    }
    else {
        // Update VERT values
        obj.setY(obj.getY() + vertComp.getDisp());
        obj.vert_motion.setTime(vertComp.getTime());
        obj.vert_motion.setVel(vertComp.getVel());
    }

    // Horizontal Component
    if (obj.getX() + horiComp.getDisp() + obj.getWidth() > screen_width) {     // Check if not too left
        obj.setX(screen_width - obj.getWidth());
        obj.hori_motion.setVel(0.0f);
    } 
    else if (obj.getX() + horiComp.getDisp() < origin_x) {                     // Check if not too right
        obj.setX(origin_x); // usually 0
        obj.hori_motion.setVel(0.0f);
    }
    else {
        // Update HORI values
        obj.setX(obj.getX() + horiComp.getDisp());
        obj.hori_motion.setTime(horiComp.getTime());
        obj.hori_motion.setVel(horiComp.getVel());
    }

    // TODO Make restrictions for circular motion
}

void PhysicsEngine::switchGravity(Object objs[], const int &objs_count, const int &elapsed_time) {
    // Invert gravity
    _gravity /= -1;

    // Reset block time
    // and set initial velocity
    for (int i = 0; i < objs_count; i++) {
        Motion motion = calcMotion(objs[i], VERT, elapsed_time);
        float miscalc_g = (float)(rand()) / (float)(RAND_MAX/_MAX_G_SWITCH_MISCALC);
        if (objs[i].vert_motion.getVel() != 0) {               // Object is moving
            objs[i].vert_motion.setAccel(_gravity);
            objs[i].vert_motion.setTime(motion.getTime());
            objs[i].vert_motion.setVel(objs[i].vert_motion.getVel() + (_gravity * motion.getChangeTime()) + miscalc_g);

        }
        else {                                                  // Object is stationary
            objs[i].vert_motion.setAccel(_gravity);
            objs[i].vert_motion.setTime(motion.getTime());
            if (_gravity < 0)
                objs[i].vert_motion.setVel(miscalc_g/-1);
            else
                objs[i].vert_motion.setVel(miscalc_g);
        }
    }
}

void PhysicsEngine::generateInitVelocity(Object &obj) {
    float init_v = _MIN_INITIAL_VERT_VELOCITY + (float)(rand()) / (float)(RAND_MAX/(_MAX_INITIAL_VERT_VELOCITY - _MIN_INITIAL_VERT_VELOCITY));
    float init_h = _MAX_INITIAL_HORI_VELOCITY + (float)(rand()) / (float)(RAND_MAX/(-_MAX_INITIAL_HORI_VELOCITY-(_MAX_INITIAL_HORI_VELOCITY)));

    obj.vert_motion.setVel(init_v);
    obj.hori_motion.setVel(init_h);

    // SDL_Log("%.4f + %.2f / (%.2f/(-%.2f - %.2f))", _MAX_INITIAL_HORI_VELOCITY, rnd, rnd_max, _MAX_INITIAL_HORI_VELOCITY, _MAX_INITIAL_HORI_VELOCITY);
    // SDL_Log("%i, x:%.2f, y:%.2f, vv:%2.f, hv:%2.f", obj.getIndex(), obj.getX(), obj.getY(), obj.vert_motion.getVel(), obj.hori_motion.getVel());
}

/* vim: set ts=4 */
