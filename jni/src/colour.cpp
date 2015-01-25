#include <stdlib.h>    // Needed for rand()
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

Colour Colour::getColour(Theme theme) {
    switch (theme) {
        case GRAY:
            return Colour(0.3f, 0.3f, 0.3f, 1.0f);

        case RED:
            return Colour(1.0f, 0.39f, 0.39f, 1.0f);

        case PURPLE:
            return Colour(0.25f, 0.195f, 0.8f, 1.0f);

        case BLUE:
            return Colour(0.167f, 0.641f, 0.8f, 1.0f);

        case RAINBOW:
            return Colour((rand() % 256) / 256.0f, 
                          (rand() % 256) / 256.0f, 
                          (rand() % 256) / 256.0f, 
                          1.0f);
    };
}
