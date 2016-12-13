/* Description: Player class is used for the user and also when playing multiplayer
 * Created On: 2014-11-25 
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "grav_object.h"
#include "point2d.h"
#include "player_renderer.h"
#include "obj_renderer.h"
#include "planet.h"
#include "action.h"
#include "texture_handler.h"
#include "theme.h"
#include "trail.h"

class Player: public Object {
    protected:
        Dir _facing;
        Action::Action _action;
        Point2D _base; // Current pos of the bottom of the player after rotations
        float _rot_offset_angle;
        float _running_speed;
        GravObject* _on_planet;
        GravObject* _last_visited_planet;
        float _closest_planet_disp;
        int _on_planet_region;  // Region of the planet that the player is on
        int _on_planet_region_prev;
        Point2D _on_planet_collision_pt;
        Point2D _running_unit_vector;
        vector<GravObject*> _orbiting_planets;

        Trail _trail;
        int _frame;
        Theme _colour_theme; // Changes the colour scheme of the boxes and other UI

    public:
        // Render player and trail
        virtual void draw(PlayerRenderer *rend, TextureHandler *tex) = 0;
        void drawTrail(ObjRenderer *rend, const vector<GravObject*> *g_objs);
        void drawStats(ObjRenderer *rend);
        vector<float> getVerticeData();


        void updatePhysics(const vector<GravObject*> *g_objs); // Do according to its current action, and update physics
        void updateDir();  // Update player facing direction
        void applyGravity();
        void resetTime(float t);

        /* Getters / Setters */
        Action::Action getAction();
        void setAction(Action::Action act);
        float getClosestPlanetDisp() const;
        void setClosestPlanetDisp(float d);
        GravObject* getOnPlanet() const;
        void setOnPlanet(GravObject* p);
        int getOnPlanetRegion() const;
        Point2D getRunningUnitVector() const;
        int getOrbitingPlanetsCount() const;
        vector<GravObject*> getOrbitingPlanets() const;
        void setOrbitingPlanets(vector<GravObject*> p);
        float getRotAngle() const;
        float getRotAngleOffset() const;
        float getRealRotAngle() const;
        void setRotAngleOffset(float angle);
        Dir getFacing();
        void setFacing(Dir dir);
        int getFrame();
        void setFrame(int frame);
        void changeTheme();

        /* Ctor - */
        Player(float x, float y, float width, float height, Theme theme);
        virtual ~Player();
};

#endif /* PLAYER_H */
