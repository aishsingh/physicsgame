#ifndef REND_UI_H
#define REND_UI_H

#include <GLES2/gl2.h>
#include "renderer.h"

class Rend_UI: public Renderer {
    private:
        std::vector<float> useObjectVertices(Object *obj);
        void setShaderData(float vertices[], float colours[], float angle);

    public:
        bool setup(int screen_w, int screen_h);
        void renderUI();
        void disableAttributes();

        /* Ctor - Init Shaders */
        Rend_UI();
};

#endif /* REND_UI_H */
