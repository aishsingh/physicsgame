/* Description: Player header
 * Created On: 2014-11-25 
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "rend_player.h"
#include "physics.h"
#include "object.h"

class Player: public Object {
    protected:
        Rend_player renderer;

        virtual void render() = 0;

    public:
        // Render player and trail
        virtual void draw() = 0;
                
        // Don according to its current action
        virtual void update(float x, float y, float angle) = 0; 
        virtual void setup() = 0;

        /* Ctor - */
        Player(float x, float y, float width, float height);
};

#endif /* PLAYER_H */
