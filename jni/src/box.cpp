#include <stdlib.h>    // Needed for rand()
#include "box.h"

Box::Box(float x, float y, float diameter, float &time, Theme &theme) : Shape(x,y,diameter,diameter,time,theme) {
    // vert_motion.setTime(time);
    // hori_motion.setTime(time);
    //
    // rot_angle = 0.0f;
    //
    // // setIndex(index);
    // useTheme(theme);
}
