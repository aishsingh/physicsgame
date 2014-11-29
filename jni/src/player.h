/* Description: Player header
 * Created On: 2014-11-25 
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "rend_player.h"
#include "physics.h"

class Player: public Object {
    protected:
        Rend_player renderer;

        void virtual render(int time, int screen_width, int screen_height) = 0;

    public:
        // Render player and trail
        void virtual draw(int time, int screen_width, int screen_height) = 0;

        // Do spaceman according to its current action
        void virtual update(float x, float y, float time) = 0; 
        void virtual setup(int screen_width, int screen_height) = 0;

        /* Ctor - */
        Player(float x, float y, float width, float height);
};

#endif /* PLAYER_H */
