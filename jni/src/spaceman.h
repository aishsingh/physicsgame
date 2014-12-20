/* Description: Spaceman header and Action enumeration
 * Created On: 2014-11-25 
 */

#ifndef SPACEMAN_H
#define SPACEMAN_H

#include "player.h"
#include "trail.h"
#include "motion.h"
#include "direction.h"

class Spaceman: public Player {
    private:
        PhysicsEngine _physics;
        Action _action;
        Dir _facing;
        int _frame;
        Theme _colour_theme; // Changes the colour scheme of the boxes and other UI
        Trail _trail;

    public:
        // Render player and trail
        // void draw();
        void drawTrail();

        // Do spaceman according to its current action
        void update(float x, float y, float angle, bool build_trail); 

        /* Getters / Setters */
        Action getAction();
        void setAction(Action act);
        Dir getFacing();
        void setFacing(Dir dir);
        int getFrame();
        void setFrame(int frame);
        void changeTheme(Theme &old_theme);

        /* Ctor - Creates a new box instance and adds it to the array */
        Spaceman(float x, float y, Theme theme, AssetRenderer* ass_rend, ObjRenderer* obj_rend);
};

#endif /* SPACEMAN_H */
