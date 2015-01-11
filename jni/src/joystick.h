/* Description: Joystick class
 * Created On: 2014-12-20 
 */

#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "rect.h"
#include "screen_renderer.h"

class Joystick : public Rect {
    private:
        vector<float> getVerticeData();

        // Size of joystick side borders
        float _size;
        // Orgin size
        float _ori_size;

    public:
        void draw(ScreenRenderer *rend);
        float getJoystickAngle(int x, int y);
        float getJoystickAngle(Point2D pos, float obj_rot_angle);

        /* Ctor */
        Joystick(float x, float y, float l, float size, float origin_size);
};

#endif /* JOYSTICK_H */
