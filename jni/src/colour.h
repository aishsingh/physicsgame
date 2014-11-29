/* Description: Colour struct holds rgba values
 * Created On: 2014-11-24 
 */
#ifndef COLOUR_H
#define COLOUR_H

struct Colour {
    float r, g, b, a;

    /* Ctor
       - Convert RGBA values into a Colour type */
    Colour() {r=0.0f, g=0.0f, b=0.0f, a=1.0f;};
    Colour(float red, float green, float blue, float alpha) {r=red; g=green; b=blue;a=alpha;};
};

#endif /* COLOUR_H */
