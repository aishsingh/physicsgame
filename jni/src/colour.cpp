#include <stdlib.h>    // Needed for rand()
#include <math.h>
#include "colour.h"

#define PI 3.14159265358979323846264

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
        case BLACK:
            return Colour(0.3f, 0.3f, 0.3f, 1.0f);

        case GREY:
            return Colour(0.41176, 0.40392f, 0.45098f, 1.0f);

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

/* Source: http://stackoverflow.com/a/8509802 */
typedef unsigned char BYTE;
BYTE clamp(float v) //define a function to bound and round the input float value to 0-255
{
    if (v < 0)
        return 0;
    if (v > 255)
        return 255;
    return (BYTE)v;
}
Colour Colour::transformHue(Colour &in, float hue)
{
    Colour out;
    const float cosA = cos(hue*PI/180); //convert degrees to radians
    const float sinA = sin(hue*PI/180); //convert degrees to radians
    //calculate the rotation matrix, only depends on Hue
    float matrix[3][3] = {{cosA + (1.0f - cosA) / 3.0f, 1.0f/3.0f * (1.0f - cosA) - sqrtf(1.0f/3.0f) * sinA, 1.0f/3.0f * (1.0f - cosA) + sqrtf(1.0f/3.0f) * sinA},
        {1.0f/3.0f * (1.0f - cosA) + sqrtf(1.0f/3.0f) * sinA, cosA + 1.0f/3.0f*(1.0f - cosA), 1.0f/3.0f * (1.0f - cosA) - sqrtf(1.0f/3.0f) * sinA},
        {1.0f/3.0f * (1.0f - cosA) - sqrtf(1.0f/3.0f) * sinA, 1.0f/3.0f * (1.0f - cosA) + sqrtf(1.0f/3.0f) * sinA, cosA + 1.0f/3.0f * (1.0f - cosA)}};
    //Use the rotation matrix to convert the RGB directly
    out.r = clamp(in.r*matrix[0][0] + in.g*matrix[0][1] + in.b*matrix[0][2]);
    out.g = clamp(in.r*matrix[1][0] + in.g*matrix[1][1] + in.b*matrix[1][2]);
    out.b = clamp(in.r*matrix[2][0] + in.g*matrix[2][1] + in.b*matrix[2][2]);
    return out;
}
