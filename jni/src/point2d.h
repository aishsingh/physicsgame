/* Description: Point2D structure
 * Created On: 2014-12-04 
 */

#ifndef POINT2D_H
#define POINT2D_H

class Point2D {
    private:
        float _x;
        float _y;

     public:
        float getX() {return _x;};
        float getY() {return _y;};
        void setX(float x) {_x=x;};
        void setY(float y) {_y=y;};

        /* Ctor */
        Point2D() {_x=0.0f, _y=0.0f;};
        Point2D(float x, float y) {_x=x, _y=y;};
};

#endif /* POINT2D_H */
