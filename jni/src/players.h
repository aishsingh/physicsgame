#ifndef PLAYERS_H
#define PLAYERS_H

#include "trail.h"
#include "object.h"
#include "motion.h"

typedef enum Action {STILL, RUNNING, FLYING} Action;

class Spaceman: public Object {
    private:

        PhysicsEngine _physics;
        // TODO SDL_image here
        Action _action;
        Dir _facing;
        int _frame;

        // Changes the colour scheme of the boxes and other UI
        Theme _colour_theme;
    public:

        Trail trail;
        Motion vert_motion, hori_motion;

        // Render player and trail
        void Draw(Renderer &renderer, int time, int screen_width, int screen_height);

        // Do spaceman according to its current action
        void update(float x, float y, float time); 

        /* Getters / Setters */
        Action getAction();
        void setAction(Action act);
        Dir getFacing();
        void setFacing(Dir dir);
        int getFrame();
        void setFrame(int frame);
        void changeTheme(Theme &old_theme);

        /* Constructor
           Check if there is once 
           Creates a new box instance and adds it to the array */
        Spaceman(const float x, const float y, const float width, const float height);
};
#endif
