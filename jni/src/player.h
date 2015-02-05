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
        Action _action;
        Point2D _base; // Current pos of the bottom of the player after rotations
        float _rot_offset_angle;
        int _on_planet;
        float _closest_planet_disp;
        unsigned _on_planets_count;

    public:
        // Render player and trail
        virtual void draw(PlayerRenderer *rend, vector<Planet*> *g_objs, TextureHandler *tex);
        virtual void drawTrail(ObjRenderer *rend, vector<Planet*> *g_objs) = 0;
        vector<float> getVerticeData();

        // Update player based on its current action
        virtual void update() = 0; 
        virtual void applyGravity(vector<Planet*> *g_objs);
        virtual void resetTime(float t);

        Action getAction();
        void setAction(Action act);
        float getClosestPlanetDisp() const;
        void setClosestPlanetDisp(float d);
        int getOnPlanet() const;
        void setOnPlanet(int index);
        unsigned getOnPlanetsCount() const;
        void setOnPlanetsCount(unsigned count);
        float getRotAngle() const;
        float getRotAngleOffset() const;
        float getRealRotAngle() const;
        void setRotAngleOffset(float angle);

        /* Ctor - */
        Player(float x, float y, float width, float height);
        virtual ~Player();
};

#endif /* PLAYER_H */
