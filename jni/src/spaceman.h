/* Description: Spaceman header and Action enumeration
 * Created On: 2014-11-25 
 */

#ifndef SPACEMAN_H
#define SPACEMAN_H

#include "player.h"
#include "trail.h"
#include "object.h"
#include "motion.h"
#include "direction.h"

typedef enum Action {STILL, RUNNING, FLYING} Action;

class Spaceman: public Player {
    private:

        PhysicsEngine _physics;
        Action _action;
        Dir _facing;
        int _frame;
        Theme _colour_theme; // Changes the colour scheme of the boxes and other UI
        Trail _trail;

        void render(int time, int screen_width, int screen_height);
    public:

        // Render player and trail
        void draw(int time, int screen_width, int screen_height);

        // Do spaceman according to its current action
        void update(float x, float y, float time); 
        void setup(int screen_width, int screen_height);

        /* Getters / Setters */
        Action getAction();
        void setAction(Action act);
        Dir getFacing();
        void setFacing(Dir dir);
        int getFrame();
        void setFrame(int frame);
        void changeTheme(Theme &old_theme);

        /* Ctor - Creates a new box instance and adds it to the array */
        Spaceman(float x, float y);
};

#endif /* SPACEMAN_H */
