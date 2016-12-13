/* Description: Spaceman header and Action enumeration
 * Created On: 2014-11-25 
 */

#ifndef SPACEMAN_H
#define SPACEMAN_H

#include "player.h"
#include "motion.h"
#include "theme.h"
#include "texture_handler.h"

class Spaceman: public Player {
    private:

    public:
        // Render player and trail
        void draw(PlayerRenderer *rend, TextureHandler *tex);

        // Do according to its current action, and update physics
        void updateAction(const vector<GravObject*> *g_objs);

        /* Ctor - Creates a new box instance and adds it to the array */
        Spaceman(float x, float y, Theme theme);
};

#endif /* SPACEMAN_H */
