#include <cmath>
#include "planet.h"
#include "colour.h"
#include "math.h"

const int Planet::_SIDES(15);
const int Planet::_GRAV_SIDES(45);
const float Planet::_GRAV_OPACITY(0.5f);
const bool Planet::_RAND_SIDES(true);
const bool Planet::_DRAW_NORMALS(true);

Planet::Planet(float x, float y, float d) : Object(x,y,d,d), _action(STILL) {
    _colour = Colour(0.9294f, 0.898f, 0.88627f, 1.0f);
    _rot_speed = powf(0.4f, (getWidth()/200));
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
        float offset = 15; //getWidth()/vertex_count;
        _vertex_offsets = Math::genRandData(vertex_count*2, 0, offset);
        _vertices = Math::removeConcaveVertices(Math::offsetDataByData(getVerticeData(vertex_count, 0), _vertex_offsets), &_vertex_offsets);
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
    setRotAngle(getRotAngle() + _rot_speed);
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
        for (int i=0; i<(int)_vertices.size(); i+=2) {
            Point2D A = Point2D(_vertices.at(i), _vertices.at(i+1));
            Point2D B = (i+2 < (int)_vertices.size()) ? 
                    Point2D(_vertices.at(i+2), _vertices.at(i+3)) :
                    Point2D(_vertices.at(0), _vertices.at(1));
            // Point2D C = Point2D(B.getY(), A.getX());

            // Normal vector (perpendicular to AB)
            Point2D N = Math::getRightNormal(A, B);

            normal.push_back(N.getX());
            normal.push_back(N.getY());

            Point2D half = Point2D((B.getX() - A.getX())/2, (B.getY() - A.getY())/2);
            Point2D mid = Point2D(A.getX() + half.getX(), A.getY() + half.getY());

            mid_vertices.push_back(mid.getX());
            mid_vertices.push_back(mid.getY());

            // Calc unit vector with axis parallel to the normal vector
            Point2D unit_vec = Math::getUnitVector(N);

            // if (i<2) {
            //     normal_vertices.push_back(mid.getX());
            //     normal_vertices.push_back(mid.getY());
            //     normal_vertices.push_back(mid.getX() + 100*unit_vec.getX());
            //     normal_vertices.push_back(mid.getY() + 100*unit_vec.getY());
            // }
            // else {
                normal_vertices.push_back(mid.getX());
                normal_vertices.push_back(mid.getY());
                normal_vertices.push_back(mid.getX() + 50*unit_vec.getX());
                normal_vertices.push_back(mid.getY() + 50*unit_vec.getY());

        }

        // draw normals
        rend->render(normal_vertices,
                Colour::getColour(RED),
                getRotAngle(), 
                GL_LINES);

        vector<float> fn_vertices;
        fn_vertices.push_back(normal_vertices.at(0));
        fn_vertices.push_back(normal_vertices.at(1));
        fn_vertices.push_back(normal_vertices.at(2));
        fn_vertices.push_back(normal_vertices.at(3));

        // first norm
        rend->render(fn_vertices,
                Colour::getColour(BLUE),
                getRotAngle(), 
                GL_LINES);
    }
}

void Planet::update() { }

void Planet::anchorObject(Object *obj) {
    Point2D pt = Math::rotatePtAroundPt(getCentre(), obj->getCentre(), _rot_speed);

    obj->setX(pt.getX() - (obj->getWidth()/2));
    obj->setY(pt.getY() - (obj->getHeight()/2));
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
