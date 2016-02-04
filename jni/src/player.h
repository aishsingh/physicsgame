/* Description: Player class is used for the user and also when playing multiplayer
 * Created On: 2014-11-25 
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"
#include "point2d.h"
#include "player_renderer.h"
#include "obj_renderer.h"
#include "planet.h"
#include "action.h"
#include "texture_handler.h"

class Player: public Object {
    protected:
        Dir _facing;
        Action::Action _action;
        Point2D _base; // Current pos of the bottom of the player after rotations
        float _rot_offset_angle;
        Planet* _on_planet;
        Planet* _last_visited_planet;
        float _closest_planet_disp;
        // unsigned _on_planets_count;
        vector<Planet*> _orbiting_planets;

    public:
        // Render player and trail
        virtual void draw(PlayerRenderer *rend, vector<Planet*> *g_objs, TextureHandler *tex);
        virtual void drawTrail(ObjRenderer *rend, vector<Planet*> *g_objs) = 0;
        void drawStats(ObjRenderer *rend, vector<Planet*> *g_objs);
        vector<float> getVerticeData();

        virtual void update() = 0; // Update player based on its current action
        void updateDir();  // Update player facing direction
        virtual void applyGravity(vector<Planet*> *g_objs);
        virtual void resetTime(float t);

        /* Getters / Setters */
        Action::Action getAction();
        void setAction(Action::Action act);
        float getClosestPlanetDisp() const;
        void setClosestPlanetDisp(float d);
        Planet* getOnPlanet() const;
        void setOnPlanet(Planet* p);
        int getOrbitingPlanetsCount() const;
        vector<Planet*> getOrbitingPlanets() const;
        void setOrbitingPlanets(vector<Planet*> p);
        float getRotAngle() const;
        float getRotAngleOffset() const;
        float getRealRotAngle() const;
        void setRotAngleOffset(float angle);

        /* Ctor - */
        Player(float x, float y, float width, float height);
        virtual ~Player();
};

#endif /* PLAYER_H */
