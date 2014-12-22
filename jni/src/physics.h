#ifndef PHYSICS_H
#define PHYSICS_H

#include "object.h"

class PhysicsEngine {
    private:
        // Pushes new boxes out with some initial velocity (up is neg, down is pos)
        const float _MAX_INIT_V;
        const float _MIN_INIT_V;
        const float _INIT_V_OFFSET;

        // Creates a more natural look of gravity when gravity is switched up/down
        const float _MAX_G_SWITCH_MISCALC;

        Motion calcMotion(Motion &motion, const float &elapsed_time);

    public:
        void updatePhysics(Object &obj, const float &elapsed_time, const int &screen_width, const int &screen_height);

        /* Reverse gravity and give blocks some initial velocity to look natural
           1. Invert gravity value
           2. Reset block time
           3. give blocks some initial velocity to make blocks fall with different levels
        void switchGravity(Object objs[], const int &boxes_count, const int &elapsed_time);
        */

        /* Generated a suitable initial velocity using rand() */
        void genInitVelocity(Object &obj, float rot_angle);

        /* Constructor */
        PhysicsEngine();
};

#endif /* PHYSICS_H */

