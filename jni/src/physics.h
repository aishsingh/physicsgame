#ifndef PHYSICS_H
#define PHYSICS_H

#include "common.h"
#include "shapes.h"

typedef enum Size {SMALL, MEDIUM, LARGE} Size;

class PhysicsEngine {
    private:
        // Pushes new boxes out with some initial velocity (up is neg, down is pos)
        float _MAX_INITIAL_VERT_VELOCITY;
        float _MIN_INITIAL_VERT_VELOCITY;
        float _MAX_INITIAL_HORI_VELOCITY;

        // Creates a more natural look of gravity when gravity is switched up/down
        float _MAX_G_SWITCH_MISCALC;
        float _gravity;   

        Motion calcMotion(Box &box, const Comp comp, const float &elapsed_time);

    public:
        void updatePhysics(Box &box, const float &elapsed_time, const int &screen_width, const int &screen_height);

        /* Reverse gravity and give blocks some initial velocity to look natural
           1. Invert gravity value
           2. Reset block time
           3. give blocks some initial velocity to make blocks fall with different levels */
        void switchGravity(Box boxes[], const int &boxes_count, const int &elapsed_time);

        /* Generated a suitable initial velocity using rand() */
        void generateInitVelocity(Box &box);

        /* Getters / Setters */
        float getGravity();

        /* Constructor */
        PhysicsEngine();
};

#endif /* PHYSICS_H */

