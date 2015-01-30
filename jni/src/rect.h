/* Description: Rectangle class used to easily pass box values
 * Created On: 2015-01-02 
 */

#ifndef RECT_H
#define RECT_H

#include "point2d.h"

class Rect {
    private:
        float _x, _y;
        float _width, _height;

    public:
        /* Getters / Setters */
        float getX() const;
        void setX(float x);
        float getY() const;
        void setY(float y);
        float getWidth() const;
        float getHeight() const;
        void setLength(float l);
        float getCentreX() const;
        float getCentreY() const;
        Point2D getCentre() const;

        /* Ctor / Dtor */
        Rect(float x, float y, float width, float height);
        virtual ~Rect();
};

#endif /* RECT_H */
