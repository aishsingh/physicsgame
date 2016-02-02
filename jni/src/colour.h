/* Description: Colour class holds rgba values
 * Created On: 2014-11-24 
 */
#ifndef COLOUR_H
#define COLOUR_H

#include <vector>
#include "theme.h"

using std::vector;

class Colour {
    public:
        float r, g, b, a;

        // Used for objects that need to be rendered
        static vector<float> getColourData(int vertex_count, Colour colour);

        static Colour getColour(Theme theme);
        static Colour transformHue(Colour &in, float hue);

        /* Ctor
           - Convert RGBA values into a Colour type */
        Colour();
        Colour(float red, float green, float blue, float alpha);
};

#endif /* COLOUR_H */
