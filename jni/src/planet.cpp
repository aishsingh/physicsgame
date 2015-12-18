#include <cmath>
#include "planet.h"
#include "colour.h"
#include "math.h"
#include "log.h"

const int Planet::_SIDES(10);
const int Planet::_GRAV_SIDES(45);
const float Planet::_GRAV_OPACITY(0.5f);
const bool Planet::_RAND_SIDES(true);
const bool Planet::_DRAW_NORMALS(true);

Planet::Planet(float x, float y, float d) : Object(x,y,d,d), _action(STILL) {
    _colour = Colour(0.9294f, 0.898f, 0.88627f, 0.6f);
    _rot_speed = 0.3f;
    _grav_radius_offset = 120.0f;
    _grav_speed = 5.5f;

    // Position grav rings
    _grav_rings_off[0] = d;
    for (int i=1; i<=2; i++) {
        float offset = _grav_radius_offset / i;
        _grav_rings_off[i] = d + offset;
    }

    // Gen initial vert data
    int vertex_count = _SIDES * getWidth()/400;
    if (_RAND_SIDES) {
        _vertex_offsets = Math::genRandData(vertex_count*2, -30.0f, 30.0f);
        _vertices = Math::offsetDataByData(getVerticeData(vertex_count, 0), _vertex_offsets);
    }
    else {
        _vertices = getVerticeData(vertex_count, 0);
    }
}
Planet::~Planet() { }

void Planet::draw(ObjRenderer *rend) {
    // Render circle
    int vertex_count = _SIDES * getWidth()/400;
    _vertices = (_RAND_SIDES) ? 
        Math::offsetDataByData(getVerticeData(vertex_count, 0), _vertex_offsets) : getVerticeData(vertex_count, 0);

    rend->render(_vertices,
                 _colour, 
                 getRotAngle(), 
                 GL_TRIANGLE_FAN);

    // Rotate
    // setRotAngle(getRotAngle() + _rot_speed);
}

void Planet::drawGrav(ObjRenderer *rend) {
    int grav_vertex_count = _GRAV_SIDES * (getWidth()/400);
    int vertex_count = _SIDES * getWidth()/400;
    int lines = (vertex_count <= 7) ? 2:3;

    // Render Gravity area with rings
    for (int i=1; i<=lines; i++) {
        _grav_rings_off[i] = (_grav_rings_off[i] <=0) ? getWidth() : _grav_rings_off[i]-=_grav_speed;

        float alpha = (_grav_rings_off[i] > getWidth()/2) ? 2 - _grav_rings_off[i]/(getWidth()/2) : _grav_rings_off[i]/(getWidth()/2);
        alpha *= _GRAV_OPACITY;

        // Render Gravity ring
        if (_grav_rings_off[i] > 0)
            rend->render(Math::offsetDataByRand(getVerticeData(grav_vertex_count, _grav_rings_off[i]), -8.0f, 8.0f),
                    Colour(_colour.r, _colour.g, _colour.b, alpha), 
                    getRotAngle(), 
                    GL_LINE_LOOP);
    }
}

void Planet::drawStats(ObjRenderer *rend, Rect circle) {
    vector<float> end_vertices;
    vector<float> proj_vertices;
    vector<float> min_vertices;
    vector<float> b_vertices;
    if (_DRAW_NORMALS) {
        vector<float> mid_vertices;
        vector<float> normal;
        vector<float> normal_vertices;
        for (int i=0; i<_vertices.size(); i+=2) {
            Point2D A = Point2D(_vertices.at(i), _vertices.at(i+1));
            Point2D B = (i+2 < _vertices.size()) ? 
                    Point2D(_vertices.at(i+2), _vertices.at(i+3)) :
                    Point2D(_vertices.at(0), _vertices.at(1));
            // Point2D C = Point2D(B.getY(), A.getX());

            Point2D AB = Point2D(B.getX() - A.getX(), B.getY() - A.getY()); 
            // Point2D AC = Point2D(C.getX() - A.getX(), C.getY() - A.getY()); 

            // Normal vector (perpendicular to AB)
            Point2D N = Point2D(AB.getY(), -AB.getX());

            normal.push_back(N.getX());
            normal.push_back(N.getY());

            Point2D half = Point2D((B.getX() - A.getX())/2, (B.getY() - A.getY())/2);
            Point2D mid = Point2D(A.getX() + half.getX(), A.getY() + half.getY());

            mid_vertices.push_back(mid.getX());
            mid_vertices.push_back(mid.getY());
        }

        for (int i=0; i<_vertices.size(); i+=2) {
        // for (int i=0; i<2; i+=2) {
            Point2D A = Point2D(_vertices.at(i), _vertices.at(i+1));
            Point2D B = (i+2 < _vertices.size()) ? 
                    Point2D(_vertices.at(i+2), _vertices.at(i+3)) :
                    Point2D(_vertices.at(0), _vertices.at(1));
            // Point2D C = Point2D(B.getY(), A.getX());

            Point2D AB = Point2D(B.getX() - A.getX(), B.getY() - A.getY()); 
            // Point2D AC = Point2D(C.getX() - A.getX(), C.getY() - A.getY()); 

            // Normal vector (perpendicular to AB)
            Point2D N = Point2D(AB.getY(), -AB.getX());

            Point2D half = Point2D((B.getX() - A.getX())/2, (B.getY() - A.getY())/2);
            Point2D mid = Point2D(A.getX() + half.getX(), A.getY() + half.getY());

            // Calc unit vector with axis parallel to the normal vector
            float mag = sqrt(pow(N.getX(), 2) + pow(N.getY(), 2));
            Point2D unit_vec = Point2D(N.getX()/mag, N.getY()/mag);

            int opposite_norm = abs(((int)_vertices.size()/2)-1);
            // LOGI("%i", opposite_norm);
            Point2D dif = Point2D(_vertices.at(opposite_norm) - mid.getX(),
                                  _vertices.at(opposite_norm+1) - mid.getY());
            float dist = getWidth();

            // if (i<2) {
                normal_vertices.push_back(mid.getX());
                normal_vertices.push_back(mid.getY());
                normal_vertices.push_back(mid.getX() + 50*unit_vec.getX());
                normal_vertices.push_back(mid.getY() + 50*unit_vec.getY());

                end_vertices.push_back(mid.getX() - dist*unit_vec.getX());
                end_vertices.push_back(mid.getY() - dist*unit_vec.getY());
                end_vertices.push_back(mid.getX());
                end_vertices.push_back(mid.getY());

                Point2D off = Point2D(mid.getX() - circle.getCentreX(), mid.getY() - circle.getCentreY());
                float of = Math::dot(off, unit_vec);

                // if (i < 2) {
                    b_vertices.push_back(mid.getX());
                    b_vertices.push_back(mid.getY());
                    b_vertices.push_back(mid.getX() - of*unit_vec.getX());
                    b_vertices.push_back(mid.getY() - of*unit_vec.getY());

                    // LOGI("dx %.2f, dy %.2f", of*unit_vec.getX(), of*unit_vec.getY());
                // }



            Point2D pt = Point2D(B.getX() + half.getX(), B.getY() + half.getY());
            Point2D v = Point2D(pt.getX() - mid.getX(), pt.getY() - mid.getY());

            float D = (v.getX()*N.getX()) + (v.getY()*N.getY());
            Point2D proj = Point2D(pt.getX() - (D*N.getX()),
                                   pt.getY() - (D*N.getY()));

            // float DP = (N.getX()*AC.getX()) + (N.getY()*AC.getY());
            // float DP = (mid.getX()*A.getX()) + (mid.getY()*A.getY());
            // Point2D proj = Point2D(DP/(B.getX()*B.getX() + B.getY()*B.getY())*B.getX(), DP*B.getY());

            if (i<2) {
                proj_vertices.push_back(mid.getX());
                proj_vertices.push_back(mid.getY());
                proj_vertices.push_back(proj.getX());
                proj_vertices.push_back(proj.getY());
            }

            // Point2D min;
            // min.setX();
            // min.setY();


            // normal_vertices.push_back(proj.getX());
            // normal_vertices.push_back(proj.getY());
            // normal.push_back(N.getX());
            // normal.push_back(N.getY());
        }

        // draw normals
        rend->render(normal_vertices,
                Colour::getColour(RED),
                getRotAngle(), 
                GL_LINES);
    }

    // rend->render(end_vertices,
    //         Colour::getColour(RED),
    //         getRotAngle(), 
    //         GL_LINES);


    // rend->render(b_vertices,
    //         Colour::getColour(BLUE),
    //         getRotAngle(), 
    //         GL_LINES);

    // draw projs
    // rend->render(proj_vertices,
    //         Colour::getColour(RED),
    //         getRotAngle(), 
    //         GL_LINES);

    /*
       for (int i=0; i<2; i+=2) {
    // Point2D A = Point2D(_vertices.at(i), _vertices.at(i+1));
    // Point2D B = (i+2 < _vertices.size()) ? 
    //         Point2D(_vertices.at(i+2), _vertices.at(i+3)) :
    //         Point2D(_vertices.at(0), _vertices.at(1));
    //
    // Point2D half = Point2D((B.getX() - A.getX())/2, (B.getY() - A.getY())/2);
    // Point2D mid = Point2D(A.getX() + half.getX(), A.getY() + half.getY());
    //

    float face_i = i + normal.size()/2;

    // Point2D A = Point2D(_vertices.at(i), _vertices.at(i+1));
    // Point2D B = (i+2 < _vertices.size()) ? 
    //         Point2D(_vertices.at(i+2), _vertices.at(i+3)) :
    //         Point2D(_vertices.at(0), _vertices.at(1));
    // // Point2D C = Point2D(B.getY(), A.getX());
    //
    // Point2D AB = Point2D(B.getX() - A.getX(), B.getY() - A.getY()); 
    // // Point2D AC = Point2D(C.getX() - A.getX(), C.getY() - A.getY()); 
    //
    // // Normal vector (perpendicular to AB)
    // Point2D N = Point2D(-AB.getY(), AB.getX());






    Point2D pt = Point2D(_vertices.at(face_i), _vertices.at(face_i+1));
    Point2D v = Point2D(pt.getX() - proj_vertices.at(i), pt.getY() - proj_vertices.at(i+1));

    Point2D norm = Point2D(normal.at(i), normal.at(i+1));

    // // normalize
    // float length = sqrt(norm.getX()*norm.getX() + norm.getY()*norm.getY());
    // norm.setX(norm.getX()/length);
    // norm.setY(norm.getY()/length);

    float D = (v.getX()*norm.getX() + (v.getY()*norm.getY()));
    Point2D proj = Point2D(pt.getX() - (D*norm.getX()),
    pt.getY() - (D*norm.getY()));



    // min_vertices.push_back(_vertices.at(i));
    // min_vertices.push_back(_vertices.at(i+1));
    // min_vertices.push_back(_vertices.at(face_i));
    // min_vertices.push_back(_vertices.at(face_i+1));

    Point2D dis = Point2D(proj_vertices.at(i) - _vertices.at(face_i),
    proj_vertices.at(i+1) - _vertices.at(face_i+1));

    // min_vertices.push_back(proj_vertices.at(i+2));
    // min_vertices.push_back(proj_vertices.at(i+3));
    // min_vertices.push_back(proj_vertices.at(i+2) - dis.getX());
    // min_vertices.push_back(proj_vertices.at(i+3) - dis.getY());
    // min_vertices.push_back(_vertices.at(face_i));
    // min_vertices.push_back(_vertices.at(face_i+1));
    //
    //
    // Point2D dis2 = Point2D(proj.getX() - _vertices.at(face_i),
    //                        proj.getY() - _vertices.at(face_i+1));


    //

    Point2D p1 = Point2D(proj_vertices.at(i+2), proj_vertices.at(i+3));
    Point2D p2 = Point2D(-proj.getX(), -proj.getY());
    Point2D p3 = Point2D(proj_vertices.at(i+2) - dis.getX(), proj_vertices.at(i+3) - dis.getY());
    Point2D p4 = Point2D(_vertices.at(face_i), _vertices.at(face_i+1));

    float m1 = (p2.getY() - p1.getY()) / (p2.getX() - p1.getX());
    float m2 = (p4.getY() - p3.getY()) / (p4.getX() - p3.getX());


    // proj bottom
    min_vertices.push_back(proj_vertices.at(i+2));
    min_vertices.push_back(proj_vertices.at(i+3));
    min_vertices.push_back(-proj.getX());
    min_vertices.push_back(-proj.getY());
    // min_vertices.push_back(_vertices.at(face_i) + normal.at(face_i) - dis2.getX());
    // min_vertices.push_back(_vertices.at(face_i+1) + normal.at(face_i+1)- dis2.getY());

    //
    // min_vertices.push_back(_vertices.at(i+1));
    // min_vertices.push_back(_vertices.at(i+2));



    // left
    min_vertices.push_back(proj_vertices.at(i+2) - dis.getX());
    min_vertices.push_back(proj_vertices.at(i+3) - dis.getY());
    min_vertices.push_back(_vertices.at(face_i));
    min_vertices.push_back(_vertices.at(face_i+1));




    // proj top
    // min_vertices.push_back(proj_vertices.at(i));
    // min_vertices.push_back(proj_vertices.at(i+1));
    // min_vertices.push_back(-proj.getX());
    // min_vertices.push_back(-proj.getY());
}
*/


    // for (int i=0; i<_vertices.size(); i+=2) {
    //     Point2D axis = Point2D(_vertices.at(i+1), _vertices.at(i));
    //     axis = Math::normalize(axis);
    //
    //     // Find the projection of the polygon on the current axis
    //     float minA = 0; float minB = 0; float maxA = 0; float maxB = 0;
    //     Math::project(axis, polygonA, ref minA, ref maxA);
    //     Math::project(axis, polygonB, ref minB, ref maxB);
    //
    //     // Check if the polygon projections are currentlty intersecting
    //     float dist;
    //     if (minA < minB) {
    //         dist = minB - maxA;
    //     } else {
    //         dist = minA - maxB;
    //     }
    //     if (dist > 0) 
    //         return false;
    // }


    // draw mins
    // rend->render(min_vertices,
    //         Colour::getColour(RED),
    //         getRotAngle(), 
    //         GL_LINES);
}

void Planet::update() { }

void Planet::anchorObject(Object *obj, Point2D base) {
    /* Set base value (bottom of the player after rotation)
     * These calculations ensure that the base pos is the same as it would be after it went through the vshader.
     * The vshader rotates the pos, so the same rotation is applyed here */
    //float rad_angle = getRotAngle()*PI/180.0f;
    Point2D dist = Point2D(base.getX() - getCentreX(), base.getY() - getCentreY());
    base.setX(base.getX() - dist.getX());
    base.setY(base.getY() - dist.getY());
    Point2D pt = Math::rotatePt(base, _rot_speed);
    obj->setX(pt.getX() + dist.getX());
    obj->setY(pt.getY() + dist.getY());


    setRotAngle(getRotAngle() + _rot_speed);
}

float Planet::getRadiusOffset() const {
    return _grav_radius_offset;
}

vector<float> Planet::getVertices() const {
    return _vertices;
}

vector<float> Planet::getVerticeData(int vertex_count, float r_offset) {
     /*    ____
          /    \
         (Circle) 
          \____/
                    */

    float radius = (getWidth()/2) + r_offset;
    float c_x = getCentreX();
    float c_y = getCentreY();

    // Create a buffer for vertex data
    vector<float> vec; // (x,y) for each vertex
    int idx = 0;

    for (int i=0; i<vertex_count; i++) {

        float x = c_x;
        float y = c_y;

        // Rotate
        Point2D pt = Math::rotatePt(Point2D(x, y), getRotAngle());
        x = pt.getX();
        y = pt.getY();

        float percent = (i / (float) (vertex_count));
        float rad = percent * 2 * PI;

        // Vertex position
        x += radius * cos(rad);
        y += radius * sin(rad);

        // Add pos
        vec.push_back(x);
        vec.push_back(y);
    }

    return vec;
}
