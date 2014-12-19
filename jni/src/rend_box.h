#ifndef REND_BOX_H
#define REND_BOX_H

#include <GLES2/gl2.h>
#include "renderer.h"
#include "shape.h"

class Rend_box: public Renderer {
    private:
        std::vector<float> useObjectVertices(Object *obj);
        void setShaderData(float vertices[], float colours[], float angle);

    public:
        void renderShape(Shape *shape);
        void disableAttributes();
        bool setup();

        /* Ctor - Init Shaders */
        Rend_box();
};

#endif /* REND_BOX_H */
