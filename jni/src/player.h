/* Description: Player header
 * Created On: 2014-11-25 
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "rend_player.h"
#include "object.h"

class Player: public Object {
    protected:
        enum Action {
            STILL, 
            RUNNING, 
            FLYING
        };

        Rend_player _renderer;

    public:
        // Render player and trail
        virtual void draw() = 0;
        virtual void drawTrail() = 0;
        // Update player based on its current action
        virtual void update(float x, float y, float angle, bool build_trail) = 0; 
        // Setup all renderers
        virtual void setup() = 0;

        /* Ctor - */
        Player(float x, float y, float width, float height);
};

#endif /* PLAYER_H */
