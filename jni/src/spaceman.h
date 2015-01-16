/* Description: Spaceman header and Action enumeration
 * Created On: 2014-11-25 
 */

#ifndef SPACEMAN_H
#define SPACEMAN_H

#include "player.h"
#include "trail.h"
#include "motion.h"
#include "theme.h"
#include "direction.h"

class Spaceman: public Player {
    private:
        Dir _facing;
        int _frame;
        Theme _colour_theme; // Changes the colour scheme of the boxes and other UI
        Trail _trail;

    public:
        // Render player and trail
        void drawTrail(ObjRenderer *_obj_rend, vector<Planet*> *g_objs, Camera *cam);

        // Do according to its current action, and update physics
        void update();

        void applyGravity(vector<Planet*> *g_objs, Camera *cam);
        void resetTime(float t);

        /* Getters / Setters */
        Action getAction();
        void setAction(Action act);
        Dir getFacing();
        void setFacing(Dir dir);
        int getFrame();
        void setFrame(int frame);
        void changeTheme(Theme &old_theme);

        /* Ctor - Creates a new box instance and adds it to the array */
        Spaceman(float x, float y, Theme theme);
};

#endif /* SPACEMAN_H */
