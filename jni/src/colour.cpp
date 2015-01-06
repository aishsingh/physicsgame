#include "colour.h"

Colour::Colour() : r(0), g(0), b(0), a(0) { }
Colour::Colour(float red, float green, float blue, float alpha) : r(red), g(green), b(blue), a(alpha) { }

vector<float> Colour::getColourData(int vertex_count, Colour colour) {
    vector<float> clr;
    for (int i=0; i<=vertex_count; i++) {
        clr.push_back(colour.r);
        clr.push_back(colour.g);
        clr.push_back(colour.b);
        clr.push_back(colour.a);
    }

    return clr;
}
