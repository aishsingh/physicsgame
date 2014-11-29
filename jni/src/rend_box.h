#ifndef REND_BOX_H
#define REND_BOX_H

#include <GLES2/gl2.h>
#include "renderer.h"
#include "shape.h"

class Rend_box: public Renderer {
    private:
        std::vector<float> useColour(Colour *colour);
        std::vector<float> useObjectVertices(Object *obj);
        void setShaderData(float vertices[], float colours[], float angle);

    public:
        bool setup(int screen_w, int screen_h);
        // void renderFrame();
        void renderShape(Shape *obj);

        /* Ctor - Init Shaders */
        Rend_box();
};

#endif /* REND_BOX_H */
