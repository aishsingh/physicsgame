/* Description: Player header
 * Created On: 2014-11-25 
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"
#include "point2d.h"

class Rend_player;

class Player: public Object {
    protected:
        enum Action {
            STILL, 
            RUNNING, 
            FLYING
        };

        Rend_player* _rend;
        Point2D _base;

    public:
        // Render player and trail
        virtual void draw() = 0;
        virtual void drawTrail() = 0;
        // Update player based on its current action
        virtual void update(float x, float y, float angle, bool build_trail) = 0; 

        // Set current pos of the bottom of the player after rotations
        void setBasePoint(Point2D point); 

        /* Ctor - */
        Player(float x, float y, float width, float height, Rend_player* rend);
};

#endif /* PLAYER_H */
