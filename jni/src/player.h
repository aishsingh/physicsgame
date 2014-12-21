/* Description: Player header
 * Created On: 2014-11-25 
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"
#include "point2d.h"
#include "asset_renderer.h"
#include "obj_renderer.h"

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
        virtual void update(float x, float y, float angle, bool build_trail) = 0; 

        // Set current pos of the bottom of the player after rotations
        void setBasePoint(Point2D point); 

        /* Ctor - */
        Player(float x, float y, float width, float height);
};

#endif /* PLAYER_H */
