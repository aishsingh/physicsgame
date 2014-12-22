#include <stdlib.h>
#include <math.h>
#include "physics.h"
#include "log.h"

#define OUT_GenInitVel false

PhysicsEngine::PhysicsEngine() : _MAX_INIT_V(30.0f), _MIN_INIT_V(26.0f), _INIT_V_OFFSET(3.0f), _MAX_G_SWITCH_MISCALC(3.0f) { }

Motion PhysicsEngine::calcMotion(Motion &motion, float const &elapsed_time) {
    ///-----------------------------------------------------------------------------
    // Physics equations of constant acceleration used to find new displacement
    // d = (final velocity * change in time) - (1/2 * acceleration * current time^2)

    float tFinal = elapsed_time;
    float ct = tFinal - motion.getTime();
    float vFinal = motion.getVel() + (motion.getAccel() * ct);
    float d = (vFinal * ct) - (0.5 * motion.getAccel() * pow(ct, 2.0));

    ///-----------------------------------------------------------------------------

    // Recalculated motion gets returned to be validated
    Motion calc;
    calc.setChangeTime(ct);
    calc.setDisp(d);
    calc.setVel(vFinal);
    calc.setTime(tFinal);
    return calc;
}

void PhysicsEngine::updatePhysics(Object &obj, const float &elapsed_time, const int &screen_width, const int &screen_height) {
    int origin_x = 0;
    int origin_y = 0;
    Motion vertComp = calcMotion(obj.vert_motion, elapsed_time);
    Motion horiComp = calcMotion(obj.hori_motion, elapsed_time);

    // Circular motion - keep rotating object
    float offset = 0.4f;
    obj.setRotAngle(obj.getRotAngle() + (obj.hori_motion.getVel() * offset));

    /* Set recalculated motion only if it validates */
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
    if (obj.getX() + horiComp.getDisp() + obj.getWidth() > screen_width) {      // Check if not too left
        obj.setX(screen_width - obj.getWidth());
        obj.hori_motion.setVel(0.0f);
    } 
    else if (obj.getX() + horiComp.getDisp() < origin_x) {                      // Check if not too right
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

/*
void PhysicsEngine::switchGravity(Object objs[], const int &objs_count, const int &elapsed_time) {
    // Invert gravity
    _gravity /= -1;

    // Reset block time
    // and set initial velocity
    for (int i = 0; i < objs_count; i++) {
        Motion motion = calcMotion(objs[i].vert_motion, elapsed_time);
        float miscalc_g = (float)(rand()) / (float)(RAND_MAX/_MAX_G_SWITCH_MISCALC);
        if (objs[i].vert_motion.getVel() != 0) {               // Object is moving
            objs[i].vert_motion.setAccel(_gravity);
            objs[i].vert_motion.setTime(motion.getTime());
            objs[i].vert_motion.setVel(objs[i].vert_motion.getVel() + (_gravity * motion.getChangeTime()) + miscalc_g);

        }
        else {                                                 // Object is stationary
            objs[i].vert_motion.setAccel(_gravity);
            objs[i].vert_motion.setTime(motion.getTime());
            if (_gravity < 0)
                objs[i].vert_motion.setVel(miscalc_g/-1);
            else
                objs[i].vert_motion.setVel(miscalc_g);
        }
    }
}
*/

void PhysicsEngine::genInitVelocity(Object &obj, float rot_angle) {
    // Remove neg values
    rot_angle /= -1;

    float init_v = 0;
    float init_h = 0;

    // Gen random velocity magnitude
    float vel = _MIN_INIT_V + (float)(rand()) / (float)(RAND_MAX/(_MAX_INIT_V - _MIN_INIT_V));
    float offset1_v = _INIT_V_OFFSET + (float)(rand()) / (float)(RAND_MAX/(-_INIT_V_OFFSET-(_INIT_V_OFFSET)));
    float offset2_v = _INIT_V_OFFSET + (float)(rand()) / (float)(RAND_MAX/(-_INIT_V_OFFSET-(_INIT_V_OFFSET)));

    // Determine velocity for both components after rotation
    float ratio = 0;
    if (rot_angle >= 0 && rot_angle < 90) {
        ratio = rot_angle / 90;

        init_v = vel * (1 - ratio);
        init_h = vel * ratio;
    }
    else if (rot_angle >= 90 && rot_angle < 180) {
        ratio = (rot_angle - 90) / 90;

        init_v = -vel * ratio;
        init_h = vel * (1 - ratio);
    }
    else if (rot_angle >= 180 && rot_angle < 270) {
        ratio = (rot_angle - 180) / 90;

        init_v = -vel * (1 - ratio);
        init_h = -vel * ratio;
    }
    else if (rot_angle >= 270 && rot_angle < 360) {
        ratio = (rot_angle - 270) / 90;

        init_v = vel * ratio;
        init_h = -vel * (1 - ratio);
    }

    // Offset
    init_v += offset1_v;
    init_h += offset2_v;

    if (OUT_GenInitVel)
        LOGI("angle %.2f, v %.2f, h %.2f", rot_angle, init_v, init_h);

    // Apply velocity
    obj.vert_motion.setVel(init_v);
    obj.hori_motion.setVel(init_h);
}

/* vim: set ts=4 */
