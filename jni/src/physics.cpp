#include <stdlib.h>
#include <cmath>
#include "physics.h"
#include "log.h"
#include "game.h"
#include "collision.h"
#include "math.h"

const float PhysicsEngine::_MAX_INIT_V(14.0f);
const float PhysicsEngine::_MIN_INIT_V(12.0f);
const float PhysicsEngine::_INIT_V_OFFSET(4.0f);
const float PhysicsEngine::_MAX_G_SWITCH_MISCALC(3.0f);

Motion PhysicsEngine::calcMotion(const Motion &motion) {
    // Physics equations of constant acceleration used to find new displacement
    // d = (final velocity * change in time) - (1/2 * acceleration * current time^2)

    float tFinal = Game::getElapsedTime();
    float ct = tFinal - motion.getTime();
    float vFinal = motion.getVel() + (motion.getAccel() * ct);
    float d = (vFinal * ct) - (0.5 * motion.getAccel() * powf(ct, 2.0));

    // Recalculated motion gets returned to be validated
    Motion calc;
    calc.setDisp(d);
    calc.setVel(vFinal);
    calc.setTime(tFinal);
    return calc;
}

Planet* PhysicsEngine::updatePhysics(Object &obj, vector<Planet*> *g_objs, int *collided_region, Point2D *collided_unit_vector) {
    /* If we dont care about the collided region or uv simplify the algorithm to speed things up.
       This is used for non-complex objects such as the boxes in a players trail */
    bool simple_mode = (collided_region == NULL || collided_unit_vector == NULL);

    int origin_x = 0;
    int origin_y = 0;
    Motion vert_comp = calcMotion(obj.vert_motion);
    Motion hori_comp = calcMotion(obj.hori_motion);

    Planet* collided_planet = NULL; // NULL means no collision
    // Rect containing updated dimensions
    Rect post_rect(obj.getX() + hori_comp.getDisp(),
                     obj.getY() + vert_comp.getDisp(),
                     obj.getWidth(),
                     obj.getHeight());

    // Check all planet collisions
    for (int i=0; i<(int)g_objs->size(); i++) {
        if (Collision::isBoundingBox(post_rect, *g_objs->at(i))) {
            bool collided = false;
            if (simple_mode) {
                if (Collision::isCircleIntersPolygon(post_rect, g_objs->at(i)->getRotAngle(), g_objs->at(i)->getVertices()))
                    collided = true;
            }
            else {
                int region = -1;
                Point2D unit_vec;
                if (Collision::isCircleIntersPolygon(post_rect, g_objs->at(i)->getRotAngle(), g_objs->at(i)->getVertices(), &region, &unit_vec)) {
                    *collided_region = region;
                    *collided_unit_vector = unit_vec;
                    collided = true;
                }
            }

            if (collided) {
                obj.hori_motion.setTime(hori_comp.getTime());
                obj.vert_motion.setTime(vert_comp.getTime());
                obj.hori_motion.setVel(0.0f);
                obj.vert_motion.setVel(0.0f);

                collided_planet = g_objs->at(i);
                break;
            }
        }
    }

    // No Collision
    if (collided_planet == NULL) {
        obj.setX(post_rect.getX());
        obj.hori_motion.setTime(hori_comp.getTime());
        obj.hori_motion.setVel(hori_comp.getVel());
        obj.setY(post_rect.getY());
        obj.vert_motion.setTime(vert_comp.getTime());
        obj.vert_motion.setVel(vert_comp.getVel());
    }

    return collided_planet;
}

void PhysicsEngine::genInitVel(Object &obj, float rot_angle, float min, float max, float offset) {
    float init_h = 0;
    float init_v = 0;

    splitCompValueFromAngle(&init_h, &init_v, rot_angle, -offset, offset, min, max);

    // Apply velocity
    obj.hori_motion.setVel(obj.hori_motion.getVel() + init_h);
    obj.vert_motion.setVel(obj.vert_motion.getVel() + init_v);
}

float PhysicsEngine::getAngleOfPtFromRectCentre(Point2D pt, Rect rect) {
    /* Quadrants
          ---      
         1   4
       |   +   |   
         2   3
          ---   */

    float angle = 0;
    float A = 0,
          O = 0;

    float radius = rect.getWidth()/2;
    float origin_x = rect.getX() + radius;
    float origin_y = rect.getY() + radius;
    float length_from_origin_x = abs(origin_x - pt.getX());
    float length_from_origin_y = abs(origin_y - pt.getY());

    // Find the quadrants
    if (pt.getX() <= rect.getX() + radius && pt.getY() < rect.getY() + radius) {
        // 1st Quad
        A += length_from_origin_y;
        O += length_from_origin_x;
    }
    else if (pt.getX() < rect.getX() + radius && pt.getY() >= rect.getY() + radius) {
        // 2st Quad
        angle += 90;
        A += length_from_origin_x;
        O += length_from_origin_y;
    }
    else if (pt.getX() >= rect.getX() + radius && pt.getY() > rect.getY() + radius) {
        // 3st Quad
        angle += 180;
        A += length_from_origin_y;
        O += length_from_origin_x;
    }
    else if (pt.getX() > rect.getX() + radius && pt.getY() <= rect.getY() + radius) {
        // 4st Quad
        angle += 270;
        A += length_from_origin_x;
        O += length_from_origin_y;
    }

    angle += atanf(O/A) * 180/PI;
    return angle;
}

void PhysicsEngine::updatePlayerOrbittingPlanets(Player &player, vector<Planet*> *g_objs) {
    vector<Planet*> orbiting_planets;
    float closest_planet_disp = 0.0f;

    for (int i=0; i<(int)g_objs->size(); i++) {
        Planet *plan = g_objs->at(i);

        // Create rect to rep the planets gravity area
        float g_radius = plan->getWidth();
        Rect grav_rect(plan->getX() - g_radius,
                       plan->getY() - g_radius,
                       plan->getWidth() + (g_radius*2),
                       plan->getWidth() + (g_radius*2));

        // Make sure obj is inside of Planet
        if (Collision::isCircleIntersCircle(player, grav_rect))
            orbiting_planets.push_back(plan);

        // Find the distance from the closest planet
        float disp = Math::getHypotenuse(fabs(player.getCentreX() - grav_rect.getCentreX()),
                                         fabs(player.getCentreY() - grav_rect.getCentreY()));
        if (disp < closest_planet_disp || closest_planet_disp == 0)
            closest_planet_disp = disp;        
    }

    player.setOrbitingPlanets(orbiting_planets);
    player.setClosestPlanetDisp(closest_planet_disp);

    // If obj was not on any planets
    if (player.getOrbitingPlanetsCount() == 0) {
        player.setAction(Action::FLYING);
        // player.setOnPlanetIndex(-1);
    }
}

void PhysicsEngine::applyGravityTo(Object &obj, vector<Planet*> *g_objs) {
    float netg_h = 0;
    float netg_v = 0;

    for (int i=0; i<(int)g_objs->size(); i++) {
        // Create rect to rep the planets gravity area
        float g_radius = g_objs->at(i)->getWidth();
        Rect grav_rect(g_objs->at(i)->getX() - g_radius,
                       g_objs->at(i)->getY() - g_radius,
                       g_objs->at(i)->getWidth() + (g_radius*2),
                       g_objs->at(i)->getWidth() + (g_radius*2));

        // Make sure obj is inside of Planet
        if (Collision::isCircleIntersCircle(obj, grav_rect)) {
            float rot_angle = getAngleOfPtFromRectCentre(obj.getCentre(), grav_rect);
            float init_v;
            float init_h;
            splitCompValueFromAngle(&init_h, &init_v, rot_angle, 0, 0, 0, 3);

            // Add to net grav
            netg_h += init_h;
            netg_v += init_v;
        }
    }
    // Apply gravity
    obj.hori_motion.setAccel(netg_h);
    obj.vert_motion.setAccel(netg_v);
}

void PhysicsEngine::applyGravityTo(Player &player, vector<Planet*> *g_objs) {
    float netg_h = 0;
    float netg_v = 0;

    if (player.getAction() == Action::STILL) {
        Planet *plan = player.getOnPlanet();

        // Create rect to rep the planets gravity area
        float g_radius = plan->getWidth();
        Rect grav_rect(plan->getX() - g_radius,
                       plan->getY() - g_radius,
                       plan->getWidth() + (g_radius*2),
                       plan->getWidth() + (g_radius*2));
        float angle_new = getAngleOfPtFromRectCentre(player.getCentre(), grav_rect);
        player.setRotAngle(-angle_new);
    }
    else {
        for (int i=0; i<(int)g_objs->size(); i++) {
            Planet *plan = g_objs->at(i);

            // Create rect to rep the planets gravity area
            float g_radius = plan->getWidth();
            Rect grav_rect(plan->getX() - g_radius,
                           plan->getY() - g_radius,
                           plan->getWidth() + (g_radius*2),
                           plan->getWidth() + (g_radius*2));

            // Rotate obj relative to planet
            float angle_new = getAngleOfPtFromRectCentre(player.getCentre(), grav_rect);

            // If player not on any planet or on different planet
            if (player.getOnPlanet() != plan) {
                player.setRotAngleOffset(-angle_new - player.getRotAngle());
                // player.setOnPlanetIndex(i);
                player.setAction(Action::LANDING);
            }


            player.setRotAngle(-angle_new);

            // Determine planet gravity from new rot angle
            float init_v;
            float init_h;
            splitCompValueFromAngle(&init_h, &init_v, angle_new, 0, 0, 0, 2.5f);

            // Add to net grav
            netg_h += init_h;
            netg_v += init_v;
        }

        if (player.getAction() != Action::STILL){
            // Or apply gravity to obj
            player.hori_motion.setAccel(netg_h);
            player.vert_motion.setAccel(netg_v);
        }
    }
}

void PhysicsEngine::splitValueFromAngle(float value, float angle, float *hori, float *vert) {
    float value_h = 0;
    float value_v = 0;

    // Determine velocity for both components after rotation
    float ratio = 0;
    if (angle >= 0 && angle < 90) {
        ratio = angle / 90;

        value_h = value * ratio;
        value_v = value * (1 - ratio);
    }
    else if (angle >= 90 && angle < 180) {
        ratio = (angle - 90) / 90;

        value_h = value * (1 - ratio);
        value_v = -value * ratio;
    }
    else if (angle >= 180 && angle < 270) {
        ratio = (angle - 180) / 90;

        value_h = -value * ratio;
        value_v = -value * (1 - ratio);
    }
    else {// if (angle >= 270 && angle < 360) {
        ratio = (angle - 270) / 90;

        value_h = -value * (1 - ratio);
        value_v = value * ratio;
    }

    // Set value
    *hori = value_h;
    *vert = value_v;
}

void PhysicsEngine::splitCompValueFromAngle(float *hori_comp, float *vert_comp, float angle, float min_hori, float max_hori, float min_vert, float max_vert) {
    // Gen random velocity magnitude
    float vel = min_vert + (float)(rand()) / (float)(RAND_MAX/(max_vert - min_vert));
    float offset1_v = min_hori + (float)(rand()) / (float)(RAND_MAX/(max_hori - min_hori));
    float offset2_v = min_hori + (float)(rand()) / (float)(RAND_MAX/(max_hori - min_hori));

    float init_v = 0;
    float init_h = 0;

    // Determine velocity for both components after rotation
    splitValueFromAngle(vel, angle, &init_h, &init_v);

    // Offset
    init_v += offset1_v;
    init_h += offset2_v;

    // Apply velocity
    *vert_comp = init_v;
    *hori_comp = init_h;
}

/* vim: set ts=4 */
