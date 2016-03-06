#include "planet.h"
#include "config.h"
#include "math.h"
#include "shape.h"
#include "collision.h"
#include "log.h"

Planet::Planet(float x, float y, float d) : GravObject(x,y,d,d), _action(STILL) {
    _colour = PLANET_COLOUR;
    if (STATS_DISABLE) 
        _colour.a = 1.0f;

    // Gen initial vert data
    int vertex_count = PLANET_AVERAGE_SIDES * getWidth()/400;
    vector<float> vert = Shape::genCircleVertices(getCentre(), getWidth()/2, getRotAngle(), vertex_count);
    if (PLANET_RAND_SIDES) {
        _vertices_offsets = Math::genRandData(vertex_count*2, 0, PLANET_RAND_SIDES_OFFSET);
        _vertices = Math::removeConcaveVertices(Math::offsetDataByData(vert, _vertices_offsets), &_vertices_offsets);
    }
    else {
        _vertices = vert;
    }
}
Planet::~Planet() { }

void Planet::draw(ObjRenderer *rend) {
    // Render circle
    rend->render(_vertices,
                 _colour, 
                 getRotAngle(), 
                 GL_TRIANGLE_FAN);

    // Rotate
    setRotAngle(getRotAngle() + _rot_speed);

    // update vertices from updated values (eg rotation, pos)
    int vertex_count = PLANET_AVERAGE_SIDES * getWidth()/400;
    vector<float> vert = Shape::genCircleVertices(getCentre(), getWidth()/2, getRotAngle(), vertex_count);
    _vertices = (PLANET_RAND_SIDES) ?  Math::offsetDataByData(vert, _vertices_offsets) : vert;
}

void Planet::drawStats(ObjRenderer *rend, bool on_planet, int collided_region) {
    vector<float> normal_vert;
    vector<float> vr_vert;
    if (STATS_PLANET_NORMALS || STATS_PLANET_FIRST_NORMAL) {
        for (int i=0; i<(int)_vertices.size(); i+=2) {
            Point2D A = Point2D(_vertices.at(i), _vertices.at(i+1));
            Point2D B = (i+2 < (int)_vertices.size()) ? 
                    Point2D(_vertices.at(i+2), _vertices.at(i+3)) :
                    Point2D(_vertices.at(0), _vertices.at(1));

            // Calc unit vector with axis parallel to the normal vector (normal is perpendicular to AB)
            Point2D unit_vec = Math::getUnitVector(Math::getNormal(A, B));

            // Calc mid point of the current edge
            Point2D mid = A + ((B - A)/2);

            normal_vert.push_back(mid.getX());
            normal_vert.push_back(mid.getY());
            normal_vert.push_back(mid.getX() + 25*unit_vec.getX());
            normal_vert.push_back(mid.getY() + 25*unit_vec.getY());

            if (on_planet) {
                vr_vert.push_back(A.getX());
                vr_vert.push_back(A.getY());
                vr_vert.push_back(A.getX() + GOBJ_REGION_HEIGHT*unit_vec.getX());
                vr_vert.push_back(A.getY() + GOBJ_REGION_HEIGHT*unit_vec.getY());
                vr_vert.push_back(B.getX());
                vr_vert.push_back(B.getY());
                vr_vert.push_back(B.getX() + GOBJ_REGION_HEIGHT*unit_vec.getX());
                vr_vert.push_back(B.getY() + GOBJ_REGION_HEIGHT*unit_vec.getY());
            }
        }
    }

    Colour c = STATS_COLOUR;
    c.a = 0.2f;

    if (STATS_PLANET_NORMALS) {
        // draw normals
        rend->render(normal_vert,
                c,
                getRotAngle(), 
                GL_LINES);
    }

    if (STATS_PLANET_FIRST_NORMAL) {
        vector<float> fn_vertices;
        fn_vertices.push_back(normal_vert.at(0));
        fn_vertices.push_back(normal_vert.at(1));
        fn_vertices.push_back(normal_vert.at(2));
        fn_vertices.push_back(normal_vert.at(3));

        // first norm
        rend->render(fn_vertices,
                Colour::getColour(RED),
                getRotAngle(), 
                GL_LINES);
    }

    if (STATS_PLANET_VR) {
        if (on_planet) {
            if (OUT_PLANET_REGION)
                LOGI("Region %i", collided_region);

            // voronoi regions
            rend->render(vr_vert,
                    c,
                    getRotAngle(), 
                    GL_LINES);

            // Highlight colliding region
            if (collided_region > -1) {
                vector<float> vr_area_vert;
                vr_area_vert.push_back(vr_vert.at(0+(8*collided_region)));
                vr_area_vert.push_back(vr_vert.at(1+(8*collided_region)));
                vr_area_vert.push_back(vr_vert.at(2+(8*collided_region)));
                vr_area_vert.push_back(vr_vert.at(3+(8*collided_region)));
                vr_area_vert.push_back(vr_vert.at(4+(8*collided_region)));
                vr_area_vert.push_back(vr_vert.at(5+(8*collided_region)));
                vr_area_vert.push_back(vr_vert.at(6+(8*collided_region)));
                vr_area_vert.push_back(vr_vert.at(7+(8*collided_region)));

                // first vr
                rend->render(vr_area_vert,
                             c,
                             getRotAngle(), 
                             GL_TRIANGLE_STRIP);
            }
        }
    }
}

vector<float> Planet::getVertices() const {
    return _vertices;
}

float Planet::getRegionAngleOffset(int region) const {
    region *= 2;
    return atanf(_vertices_offsets.at(region+1) / _vertices_offsets.at(region)) * (180/PI);
}
