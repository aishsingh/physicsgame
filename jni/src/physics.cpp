#include <stdlib.h>
#include <cmath>
#include "physics.h"
#include "config.h"
#include "game.h"
#include "math.h"

Motion Physics::calcMotion(const Motion &motion, float elapsed_time) {
    // Physics equations of constant acceleration used to find new displacement
    // d = (final velocity * change in time) - (1/2 * acceleration * current time^2)

    float tFinal = elapsed_time;
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

GravObject* Physics::updatePhysicsForCollisions(Object *obj, const vector<GravObject*> *g_objs, float elapsed_time) {
    Motion vert_comp = calcMotion(obj->vert_motion, elapsed_time);
    Motion hori_comp = calcMotion(obj->hori_motion, elapsed_time);
    GravObject* collided_planet = NULL; // NULL means no collision

    // Generate rect containing updated dimensions
    Rect post_rect(obj->getX() + hori_comp.getDisp(),
                   obj->getY() + vert_comp.getDisp(),
                   obj->getWidth(),
                   obj->getHeight());

    // Check all planet collisions
    for (int i=0; i<(int)g_objs->size(); i++) {
        if (Collision::isBoundingBox(post_rect, *g_objs->at(i))) {
            if (Collision::isCircleIntersPolygon(post_rect, g_objs->at(i)->getRotAngle(), g_objs->at(i)->getVertices())) {
                obj->hori_motion.setTime(hori_comp.getTime());
                obj->vert_motion.setTime(vert_comp.getTime());
                obj->hori_motion.setVel(0.0f);
                obj->vert_motion.setVel(0.0f);

                collided_planet = g_objs->at(i);
                break;
            }
        }
    }

    // No Collisions
    // So actually update physics attributes
    if (!collided_planet) {
        obj->setX(post_rect.getX());
        obj->setY(post_rect.getY());
        obj->hori_motion.setTime(hori_comp.getTime());
        obj->hori_motion.setVel(hori_comp.getVel());
        obj->vert_motion.setTime(vert_comp.getTime());
        obj->vert_motion.setVel(vert_comp.getVel());
    }

    return collided_planet;
}

void Physics::genInitVel(Object &obj, float rot_angle, float min, float max, float offset) {
    float init_h = 0;
    float init_v = 0;

    splitCompValueFromAngle(&init_h, &init_v, rot_angle, -offset, offset, min, max);

    // Apply velocity
    obj.hori_motion.setVel(obj.hori_motion.getVel() + init_h);
    obj.vert_motion.setVel(obj.vert_motion.getVel() + init_v);
}

unsigned Physics::getQuadOfPtAroundRect(Point2D pt, Rect rect) {
    /* Quadrants
          ---      
         1   4
       |   +   |   
         2   3
          ---   */

    float radius = rect.getWidth()/2;

    // Find the quadrants
    if (pt.getX() <= rect.getX() + radius && pt.getY() < rect.getY() + radius)
        return 1;
    else if (pt.getX() < rect.getX() + radius && pt.getY() >= rect.getY() + radius)
        return 2;
    else if (pt.getX() >= rect.getX() + radius && pt.getY() > rect.getY() + radius)
        return 3;
    else
        return 4;
}

float Physics::getAngleOfPtAroundRect(Point2D pt, Rect rect) {
    float angle = 0;
    float A = 0,
          O = 0;

    float radius = rect.getWidth()/2;
    float origin_x = rect.getX() + radius;
    float origin_y = rect.getY() + radius;
    float length_from_origin_x = abs(origin_x - pt.getX());
    float length_from_origin_y = abs(origin_y - pt.getY());

    // Find the quadrants
    unsigned in_quad = getQuadOfPtAroundRect(pt, rect);

    switch (in_quad) {
        case 1:
            A += length_from_origin_y;
            O += length_from_origin_x;
            break;
            
        case 2:
            angle += 90;
            A += length_from_origin_x;
            O += length_from_origin_y;
            break;

        case 3:
            angle += 180;
            A += length_from_origin_y;
            O += length_from_origin_x;
            break;

        case 4:
            angle += 270;
            A += length_from_origin_x;
            O += length_from_origin_y;
            break;
    }

    angle += atanf(O/A) * (180/PI);
    return angle;
}

void Physics::updatePlayerOrbittingPlanets(Player &player, const vector<GravObject*> *g_objs) {
    vector<GravObject*> orbiting_planets;
    float closest_planet_disp = 0.0f;

    for (int i=0; i<(int)g_objs->size(); i++) {
        GravObject *plan = g_objs->at(i);

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
    }
}

void Physics::applyGravityTo(Object &obj, const vector<GravObject*> *g_objs) {
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
            float rot_angle = getAngleOfPtAroundRect(obj.getCentre(), grav_rect);
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

void Physics::applyGravityTo(Player &player, const vector<GravObject*> *g_objs) {
    float netg_h = 0;
    float netg_v = 0;

    if (player.getAction() == Action::STILL || player.getAction() == Action::RUNNING) {
        GravObject *plan = player.getOnPlanet();

        // Create rect to rep the planets gravity area
        float g_radius = plan->getWidth();
        Rect grav_rect(plan->getX() - g_radius,
                       plan->getY() - g_radius,
                       plan->getWidth() + (g_radius*2),
                       plan->getWidth() + (g_radius*2));

        float angle_new;
        int cur_region = player.getOnPlanetRegion();
        if (cur_region % 2 == 0) {  // Sit flat on a regions surface
            Point2D unit_vec = player.getRunningUnitVector();
            angle_new = atanf(-unit_vec.getY()/unit_vec.getX()) * (180/PI);

            // Allow player to flip upside down when the running dir is upside down
            if (unit_vec.getX() < 0 || (unit_vec.getX() < 0 && unit_vec.getY() < 0))
                angle_new -= (angle_new > 0) ? 180: -180;
                
            /* Normalize angle
             * Source: http://stackoverflow.com/a/2323034/3270542 */
            angle_new = fmod(angle_new, 360);       // reduce the angle  
            angle_new = fmod(angle_new + 360, 360); // force it to be the positive remainder, so that 0 <= angle < 360  
        }
        else
            angle_new = getAngleOfPtAroundRect(player.getCentre(), grav_rect);

        // Determine planet gravity from new rot angle
        if (player.getAction() == Action::RUNNING && cur_region != -1)
            splitCompValueFromAngle(&netg_h, &netg_v, angle_new, 0, 0, 0, 2.5f);

        player.setRotAngle(-angle_new);
    }
    else {
        for (int i=0; i<(int)g_objs->size(); i++) {
            GravObject *plan = g_objs->at(i);

            // Create rect to rep the planets gravity area
            float g_radius = plan->getWidth();
            Rect grav_rect(plan->getX() - g_radius,
                           plan->getY() - g_radius,
                           plan->getWidth() + (g_radius*2),
                           plan->getWidth() + (g_radius*2));

            // Rotate obj relative to planet
            float angle_new = getAngleOfPtAroundRect(player.getCentre(), grav_rect);

            // If player not on any planet or on different planet
            if (player.getOnPlanet() != plan) {
                player.setRotAngleOffset(-angle_new - player.getRotAngle());
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
    }

    if (player.getAction() != Action::STILL) {
        // Or apply gravity to obj
        player.hori_motion.setAccel(netg_h);
        player.vert_motion.setAccel(netg_v);
    }
}

void Physics::splitValueFromAngle(float value, float angle, float *hori, float *vert) {
    float value_h = 0;
    float value_v = 0;

    // Determine velocity for both components after rotation
    // TODO find a way to replace if statement blocks with trig
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

void Physics::splitCompValueFromAngle(float *hori_comp, float *vert_comp, float angle, float min_hori, float max_hori, float min_vert, float max_vert) {
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
