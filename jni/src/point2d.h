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
        float getX() const {return _x;};
        float getY() const {return _y;};
        void setX(float x) {_x=x;};
        void setY(float y) {_y=y;};

        /* Operators */
        Point2D operator-(const Point2D &in) { return Point2D(_x - in.getX(), _y - in.getY()); }
        Point2D operator-(const float &in)   { return Point2D(_x - in, _y - in); }
        Point2D operator+(const Point2D &in) { return Point2D(_x + in.getX(), _y + in.getY()); }
        Point2D operator+(const float &in)   { return Point2D(_x + in, _y + in); }
        Point2D operator*(const Point2D &in) { return Point2D(_x * in.getX(), _y * in.getY()); }
        Point2D operator*(const float &in)   { return Point2D(_x * in, _y * in); }
        Point2D operator/(const Point2D &in) { return Point2D(_x / in.getX(), _y / in.getY()); }
        Point2D operator/(const float &in)   { return Point2D(_x / in, _y / in); }

        /* Ctor */
        Point2D() {_x=0.0f, _y=0.0f;};
        Point2D(float x, float y) {_x=x, _y=y;};
};

#endif /* POINT2D_H */
