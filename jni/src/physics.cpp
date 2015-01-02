#include <stdlib.h>
#include <math.h>
#include "physics.h"
#include "log.h"
#include "game.h"
#include "ui.h"

PhysicsEngine::PhysicsEngine() {}

const float PhysicsEngine::_MAX_INIT_V(8.0f);
const float PhysicsEngine::_MIN_INIT_V(6.0f);
const float PhysicsEngine::_INIT_V_OFFSET(1.0f);
const float PhysicsEngine::_MAX_G_SWITCH_MISCALC(3.0f);

Motion PhysicsEngine::calcMotion(const Motion &motion) {
    ///-----------------------------------------------------------------------------
    // Physics equations of constant acceleration used to find new displacement
    // d = (final velocity * change in time) - (1/2 * acceleration * current time^2)

    float tFinal = Game::getElapsedTime();
    float ct = tFinal - motion.getTime();
    float vFinal = motion.getVel() + (motion.getAccel() * ct);
    float d = (vFinal * ct) - (0.5 * motion.getAccel() * powf(ct, 2.0));

    ///-----------------------------------------------------------------------------

    // Recalculated motion gets returned to be validated
    Motion calc;
    calc.setDisp(d);
    calc.setVel(vFinal);
    calc.setTime(tFinal);
    return calc;
}

void PhysicsEngine::updatePhysics(Object &obj) {
    int origin_x = 0;
    int origin_y = 0;
    Motion vertComp = calcMotion(obj.vert_motion);
    Motion horiComp = calcMotion(obj.hori_motion);

    // Circular motion - keep rotating object
    float offset = 0.4f;
    obj.setRotAngle(obj.getRotAngle() + (obj.hori_motion.getVel() * offset));

    /* Set recalculated motion only if it validates */
    // Vertical Component
    if (obj.getY() + vertComp.getDisp() + obj.getHeight() > Game::getScreenHeight()) {    // Check if not too high
        obj.setY(Game::getScreenHeight() - obj.getHeight());
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
    if (obj.getX() + horiComp.getDisp() + obj.getWidth() > Game::getScreenWidth()) {      // Check if not too left
        obj.setX(Game::getScreenWidth() - obj.getWidth());
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

    splitCompValueFromAngle(&init_h, &init_v, rot_angle, -_INIT_V_OFFSET, _INIT_V_OFFSET, _MIN_INIT_V, _MAX_INIT_V);

    // Apply velocity
    obj.vert_motion.setVel(init_v);
    obj.hori_motion.setVel(init_h);
}

float PhysicsEngine::getAngleOfPtFromObjCentre(float x, float y, float obj_x, float obj_y, float obj_length) {
    float angle = 0;

    // Check if pt is actually inside Object
    if (x >= obj_x && x <= obj_x + obj_length && 
            y >= obj_y && y <= obj_y + obj_length) {

        /* Quadrants
             ---      
            1   4
          |   +   |   
            2   3
             ---   */

        float A = 0,
              O = 0;

        float origin_x = obj_x + (obj_length/2);
        float origin_y = obj_y + (obj_length/2);
        float length_from_origin_x = 0;
        float length_from_origin_y = 0;
        if (x >= origin_x)
            length_from_origin_x = origin_x - x;
        else
            length_from_origin_x = x - origin_x;
        if (y >= origin_y)
            length_from_origin_y = origin_y - y;
        else
            length_from_origin_y = y - origin_y;

        // Find out the quadrants
        if (x <= obj_x + (obj_length/2) && y < obj_y + (obj_length/2)) {
            // 1st Quad
            A += length_from_origin_y;
            O += length_from_origin_x;
        }
        else if (x < obj_x + (obj_length/2) && y >= obj_y + (obj_length/2)) {
            // 2st Quad
            angle += 90;
            A += length_from_origin_x;
            O += length_from_origin_y;
        }
        else if (x >= obj_x + (obj_length/2) && y > obj_y + (obj_length/2)) {
            // 3st Quad
            angle += 180;
            A += length_from_origin_y;
            O += length_from_origin_x;
        }
        else if (x > obj_x + (obj_length/2) && y <= obj_y + (obj_length/2)) {
            // 4st Quad
            angle += 270;
            A += length_from_origin_x;
            O += length_from_origin_y;
        }

        angle += atanf(O/A) * 180/PI;
    }
    else {
        // Pt is not inside of object
        angle = -1;
    }
    return angle;

}

void PhysicsEngine::applyGravityTo(Object &obj, vector<Planet*> *g_objs) {
    float netg_h = 0;
    float netg_v = 0;

    for (int i=0; i<(int)g_objs->size(); i++) {
        float g_radius = g_objs->at(i)->getWidth()/2;
        float rot_angle = getAngleOfPtFromObjCentre(obj.getCentreX(), 
                                                 obj.getCentreY(), 
                                                 g_objs->at(i)->getX() - g_radius,
                                                 g_objs->at(i)->getY() - g_radius,
                                                 g_objs->at(i)->getWidth() + (g_radius*2));
        if (rot_angle != 0) {
            float init_v = 0;
            float init_h = 0;

            splitCompValueFromAngle(&init_h, &init_v, rot_angle, 0, 0, 0, 2);

            netg_h += init_h;
            netg_v += init_v;
        }
    }
    // Apply gravity
    obj.hori_motion.setAccel(netg_h);
    obj.vert_motion.setAccel(netg_v);
}

void PhysicsEngine::splitCompValueFromAngle(float *hori_comp, float *vert_comp, float angle, float min_hori, float max_hori, float min_vert, float max_vert) {
    float init_v = 0;
    float init_h = 0;

    // Gen random velocity magnitude
    float vel = min_vert + (float)(rand()) / (float)(RAND_MAX/(max_vert - min_vert));
    float offset1_v = min_hori + (float)(rand()) / (float)(RAND_MAX/(max_hori - min_hori));
    float offset2_v = min_hori + (float)(rand()) / (float)(RAND_MAX/(max_hori - min_hori));

    // Determine velocity for both components after rotation
    float ratio = 0;
    if (angle >= 0 && angle < 90) {
        ratio = angle / 90;

        init_v = vel * (1 - ratio);
        init_h = vel * ratio;
    }
    else if (angle >= 90 && angle < 180) {
        ratio = (angle - 90) / 90;

        init_v = -vel * ratio;
        init_h = vel * (1 - ratio);
    }
    else if (angle >= 180 && angle < 270) {
        ratio = (angle - 180) / 90;

        init_v = -vel * (1 - ratio);
        init_h = -vel * ratio;
    }
    else if (angle >= 270 && angle < 360) {
        ratio = (angle - 270) / 90;

        init_v = vel * ratio;
        init_h = -vel * (1 - ratio);
    }

    // Offset
    init_v += offset1_v;
    init_h += offset2_v;

    // Apply velocity
    *vert_comp = init_v;
    *hori_comp = init_h;
}

/* vim: set ts=4 */
