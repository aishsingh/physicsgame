#ifndef PLAYERS_H
#define PLAYERS_H

#include "object.h"
#include "motion.h"

typedef enum Action {STILL, RUNNING, FLYING} Action;

class Spaceman: public Object {
    private:
        // TODO SDL_image here
        Action _action;
        Dir _facing;
        int _frame;
    public:
        Motion vert_motion, hori_motion;
        /* Getters / Setters */
        Action getAction();
        void setAction(Action act);
        Dir getFacing();
        void setFacing(Dir dir);
        int getFrame();
        void setFrame(int frame);


        /* Constructor
           Check if there is once 
           Creates a new box instance and adds it to the array */
        Spaceman(const float x, const float y, const float width, const float height);
};
#endif
