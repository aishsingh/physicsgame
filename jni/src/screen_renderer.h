#ifndef SCREEN_RENDERER_H
#define SCREEN_RENDERER_H

#include <GLES2/gl2.h>
#include "renderer.h"

class ScreenRenderer: public Renderer {
    private:
        GLuint _vColor_handle;

    public:
        void render(vector<float> vertices, vector<float> colours, float angle, GLenum mode);
        void disableAttributes();

        /* Ctor - Init Shaders */
        ScreenRenderer();
};

#endif /* SCREEN_RENDERER_H */
