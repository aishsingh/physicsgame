#include <stdlib.h>                // Needed for rand()
#include <math.h>                  // Needed for math functions

#include "physics.h"
#include "common.h"
#include "shapes.h"
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

Motion PhysicsEngine::calcMotion(Box &box, const Comp comp, const float &elapsed_time) {
    Motion calc;

    if (comp == VERT) {
        ///-----------------------------------------------------------------------------
        // Physics equations of constant acceleration used to find new displacement
        // d = (final velocity * change in time) - (1/2 * acceleration * current time^2)


        float tFinal = elapsed_time;
        float ct = tFinal - box.vert_motion.getTime();
        float vFinal = box.vert_motion.getVel() + (box.vert_motion.getAccel() * ct);
        float d = (vFinal * ct) - (0.5 * box.vert_motion.getAccel() * pow(ct, 2.0));
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
        // float ct = tFinal - box.hori_motion.getTime();
        // float vFinal = box.hori_motion.getVel();
        // float d = (vFinal * ct);
        float tFinal = elapsed_time;
        float ct = tFinal - box.hori_motion.getTime();
        float vFinal = box.hori_motion.getVel() + (box.hori_motion.getAccel() * ct);
        float d = (vFinal * ct) - (0.5 * box.hori_motion.getAccel() * pow(ct, 2.0));
        // SDL_Log("tFinal:%.4f, ct:%.4f, vFinal:%.4f, d:%.4f", tFinal, ct, vFinal, d);

        ///-----------------------------------------------------------------------------
        if (box.vert_motion.getAccel() == 0) {
            // if (box.hori_motion.getVel() > 0)
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
        box.rot_angle += box.hori_motion.getVel() * offset;

        // LOGI("angle [%.2f]", box.rot_angle);
    }

    return calc;
}

void PhysicsEngine::updatePhysics(Box &box, const float &elapsed_time, const int &screen_width, const int &screen_height) {
    int origin_x = 0;//screen_width/-1;  // usually 0
    int origin_y = 0;//screen_height/-1; // usually 0
    Motion vertComp = calcMotion(box, VERT, elapsed_time);
    Motion horiComp = calcMotion(box, HORI, elapsed_time);
    Motion circComp = calcMotion(box, CIRC, elapsed_time);

    // Vertical Component
    if (box.getY() + vertComp.getDisp() + box.getHeight() > screen_height) {    // Check if not too high
        box.setY(screen_height - box.getHeight());
        box.vert_motion.setVel(0.0f);
    } 
    else if (box.getY() + vertComp.getDisp() < origin_y) {                      // Check if not too low
        box.setY(origin_y); // usually 0
        box.vert_motion.setVel(0.0f);
    }
    else {
        // Update VERT values
        box.setY(box.getY() + vertComp.getDisp());
        box.vert_motion.setTime(vertComp.getTime());
        box.vert_motion.setVel(vertComp.getVel());
    }

    // Horizontal Component
    if (box.getX() + horiComp.getDisp() + box.getWidth() > screen_width) {     // Check if not too left
        box.setX(screen_width - box.getWidth());
        box.hori_motion.setVel(0.0f);
    } 
    else if (box.getX() + horiComp.getDisp() < origin_x) {                     // Check if not too right
        box.setX(origin_x); // usually 0
        box.hori_motion.setVel(0.0f);
    }
    else {
        // Update HORI values
        box.setX(box.getX() + horiComp.getDisp());
        box.hori_motion.setTime(horiComp.getTime());
        box.hori_motion.setVel(horiComp.getVel());
    }

    // TODO Make restrictions for circular motion
}

void PhysicsEngine::switchGravity(Box boxes[], const int &boxes_count, const int &elapsed_time) {
    // Invert gravity
    _gravity /= -1;

    // Reset block time
    // and set initial velocity
    for (int i = 0; i < boxes_count; i++) {
        Motion motion = calcMotion(boxes[i], VERT, elapsed_time);
        float miscalc_g = (float)(rand()) / (float)(RAND_MAX/_MAX_G_SWITCH_MISCALC);
        if (boxes[i].vert_motion.getVel() != 0) {               // Box is moving
            boxes[i].vert_motion.setAccel(_gravity);
            boxes[i].vert_motion.setTime(motion.getTime());
            boxes[i].vert_motion.setVel(boxes[i].vert_motion.getVel() + (_gravity * motion.getChangeTime()) + miscalc_g);

        }
        else {                                                  // Box is stationary
            boxes[i].vert_motion.setAccel(_gravity);
            boxes[i].vert_motion.setTime(motion.getTime());
            if (_gravity < 0)
                boxes[i].vert_motion.setVel(miscalc_g/-1);
            else
                boxes[i].vert_motion.setVel(miscalc_g);
        }
    }
}

void PhysicsEngine::generateInitVelocity(Box &box) {
    float init_v = _MIN_INITIAL_VERT_VELOCITY + (float)(rand()) / (float)(RAND_MAX/(_MAX_INITIAL_VERT_VELOCITY - _MIN_INITIAL_VERT_VELOCITY));
    float init_h = _MAX_INITIAL_HORI_VELOCITY + (float)(rand()) / (float)(RAND_MAX/(-_MAX_INITIAL_HORI_VELOCITY-(_MAX_INITIAL_HORI_VELOCITY)));

    box.vert_motion.setVel(init_v);
    box.hori_motion.setVel(init_h);

    // SDL_Log("%.4f + %.2f / (%.2f/(-%.2f - %.2f))", _MAX_INITIAL_HORI_VELOCITY, rnd, rnd_max, _MAX_INITIAL_HORI_VELOCITY, _MAX_INITIAL_HORI_VELOCITY);
    // SDL_Log("%i, x:%.2f, y:%.2f, vv:%2.f, hv:%2.f", box.getIndex(), box.getX(), box.getY(), box.vert_motion.getVel(), box.hori_motion.getVel());
}

/* vim: set ts=4 */
