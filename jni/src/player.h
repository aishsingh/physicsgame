/* Description: Player header
 * Created On: 2014-11-25 
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"
#include "point2d.h"
#include "asset_renderer.h"
#include "obj_renderer.h"
#include "planet.h"

class Player: public Object {
    protected:
        enum Action {
            STILL, 
            RUNNING, 
            FLYING
        };

        Action _action;
        Point2D _base;

    public:
        // Render player and trail
        virtual void draw(AssetRenderer* _ass_rend);
        virtual void drawTrail(ObjRenderer* _obj_rend) = 0;
        vector<float> getVerticeData();
        vector<float> getColourData();

        // Update player based on its current action
        virtual void update() = 0; 

        virtual void applyGravity(vector<Planet*> *g_objs);
        virtual void resetTime();

        // Set current pos of the bottom of the player after rotations
        void setBasePoint(Point2D point); 

        /* Ctor - */
        Player(float x, float y, float width, float height);
};

#endif /* PLAYER_H */
