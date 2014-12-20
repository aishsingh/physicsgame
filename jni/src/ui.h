/* Description: All user interface code here
 * Created On: 2014-12-20 
 */

#ifndef UI_H
#define UI_H

#include "screen_renderer.h"

class UI {
    private:
        vector<float> getVerticeData();
        vector<float> getColourData();

    public:
        void draw(ScreenRenderer *rend);

        /* Ctor */
        UI();
};

#endif /* UI_H */
