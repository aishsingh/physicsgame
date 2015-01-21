/* Description: Player class is used for the user and also when playing multiplayer
 * Created On: 2014-11-25 
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"
#include "point2d.h"
#include "asset_renderer.h"
#include "obj_renderer.h"
#include "planet.h"
#include "action.h"

class Player: public Object {
    protected:
        Action _action;
        Point2D _base;
        float _rot_offset_angle;
        int _on_planet;
        float _closest_planet_disp;

    public:
        // Render player and trail
        virtual void draw(AssetRenderer *_ass_rend, vector<Planet*> *g_objs);
        virtual void drawTrail(ObjRenderer *_obj_rend, vector<Planet*> *g_objs) = 0;
        vector<float> getVerticeData();
        vector<float> getColourData();

        // Update player based on its current action
        virtual void update() = 0; 
        virtual void applyGravity(vector<Planet*> *g_objs);
        virtual void resetTime(float t);

        // Set current pos of the bottom of the player after rotations
        void setBasePoint(Point2D point); 
        float getClosestPlanetDisp() const;
        void setClosestPlanetDisp(float d);
        int getOnPlanet() const;
        void setOnPlanet(int index);
        float getRotAngle() const;
        float getRotAngleOffset() const;
        float getRealRotAngle() const;
        void setRotAngleOffset(float angle);

        /* Ctor - */
        Player(float x, float y, float width, float height);
        virtual ~Player();
};

#endif /* PLAYER_H */
