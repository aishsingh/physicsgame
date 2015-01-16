/* Description: Input handler class gets data from java through JNI
 * Created On: 2015-01-11 
 */

#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <vector>
#include "point2d.h"
#include "player.h"
#include "camera.h"
#include "component.h"

class InputHandler {
    private:
        struct Touch {
            Point2D pos;
            unsigned int index;

            // Ctor
            Touch(float x, float y, unsigned int i) : pos(x, y), index(i) {};
        };
        std::vector<Touch> _touches;
        Player *_user; // Needed to rotate player
        Camera *_cam;

        Comp _nav_axis;
        bool _nav_active;
        bool _nav_left_hand;
        Point2D _nav_starting_pt;
        float _nav_starting_angle;
        unsigned int _nav_touch_index;

    public:

        /* Functions called from game class */
        Point2D getTouchPos(unsigned int index) const;
        unsigned int getCount() const;
        bool isNavActive() const;

        /* Events called from JNI (via app_glue.cpp) */
        void touchDown(float x, float y, unsigned int index);
        void touchPointerDown(float x, float y, unsigned int index);
        void touchMove(float x, float y, unsigned int index);
        void touchPointerUp(unsigned int index);
        void touchUp(unsigned int index);

        /* Ctor */
        InputHandler(bool nav_left_hand, Comp nav_axis, Player *user, Camera *cam);
};

#endif /* INPUT_HANDLER_H */
