/* Description: Input handler class gets data from java through JNI
 * Created On: 2015-01-11 
 */

#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <vector>
#include "point2d.h"
#include "planet.h"
#include "player.h"
#include "camera.h"
#include "component.h"

class InputHandler {
    private:
        struct Touch {
            Point2D pos;

            // Ctor
            Touch(float x, float y) : pos(x, y) {};
        };
        std::vector<Touch> _touches;
        Player *_user; // Needed to rotate player
        Camera *_cam;

        Comp _nav_axis;
        bool _nav_left_hand;
        bool _nav_active;
        Point2D _nav_starting_pt;
        unsigned _nav_touch_index;
        float _nav_starting_angle;
        float _nav_starting_real_angle;
        vector<Planet*> _nav_started_on_planets;

        void restartValues(Point2D pos, int index);

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
