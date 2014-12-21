#include <stdlib.h>
#include <math.h>

#include "physics.h"
#include "log.h"

PhysicsEngine::PhysicsEngine() {
    /* Initialize */
    _MAX_INITIAL_VERT_VELOCITY = -30.0f;
    _MIN_INITIAL_VERT_VELOCITY = -26.0f;
    _MAX_INITIAL_HORI_VELOCITY = 3.0f;
    _MAX_G_SWITCH_MISCALC = 3.0f;
    _gravity = 0.98f;
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
        float offset = 0.4f;
        obj.setRotAngle(obj.getRotAngle() + (obj.hori_motion.getVel() * offset));
    }

    return calc;
}

void PhysicsEngine::updatePhysics(Object &obj, const float &elapsed_time, const int &screen_width, const int &screen_height) {
    int origin_x = 0;
    int origin_y = 0;
    Motion vertComp = calcMotion(obj, VERT, elapsed_time);
    Motion horiComp = calcMotion(obj, HORI, elapsed_time);
    Motion circComp = calcMotion(obj, CIRC, elapsed_time);    // keeps rotating object

    // Vertical Component
    if (obj.getY() + vertComp.getDisp() + obj.getHeight() > screen_height) {    // Check if not too high
        obj.setY(screen_height - obj.getHeight());
        obj.vert_motion.setVel(0.0f);
    } 
    else if (obj.getY() + vertComp.getDisp() < origin_y) {                      // Check if not too low
        obj.setY(origin_y);
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

void PhysicsEngine::generateInitVelocity(Object &obj, float rot_angle) {
    // Remove neg values
    rot_angle /= -1;
    // rot_angle = (360 - (rot_angle/-1)) /-1;

    float init_v = 0;
    float init_h = 0;

    // Gen random velocity magnitude
    float dominantV = -1 * _MIN_INITIAL_VERT_VELOCITY + (float)(rand()) / (float)(RAND_MAX/(_MAX_INITIAL_VERT_VELOCITY - _MIN_INITIAL_VERT_VELOCITY));
    float sideV = _MAX_INITIAL_HORI_VELOCITY + (float)(rand()) / (float)(RAND_MAX/(-_MAX_INITIAL_HORI_VELOCITY-(_MAX_INITIAL_HORI_VELOCITY)));

    // Determine velocity for both components after rotation
    float ratio = 0;
    if (rot_angle >= 0 && rot_angle < 90) {
        ratio = rot_angle / 90;

        init_v = dominantV * (1 - ratio);
        init_h = dominantV * ratio;
    }
    else if (rot_angle >= 90 && rot_angle < 180) {
        ratio = (rot_angle - 90) / 90;

        init_v = -dominantV * ratio;
        init_h = dominantV * (1 - ratio);
    }
    else if (rot_angle >= 180 && rot_angle < 270) {
        ratio = (rot_angle - 180) / 90;

        init_v = -dominantV * (1 - ratio);
        init_h = -dominantV * ratio;
    }
    else if (rot_angle >= 270 && rot_angle < 360) {
        ratio = (rot_angle - 270) / 90;

        init_v = dominantV * ratio;
        init_h = -dominantV * (1 - ratio);
    }

    // Even more offset
    init_v += sideV;
    init_h += sideV;

    // Apply velocity
    obj.vert_motion.setVel(init_v);
    obj.hori_motion.setVel(init_h);
    obj.vert_motion.setAccel(getGravity());
}

/* vim: set ts=4 */
