#ifndef BACKGROUND_RENDERER_H
#define BACKGROUND_RENDERER_H

#include <GLES2/gl2.h>
#include "renderer.h"

class BackgroundRenderer: public Renderer {
    private:
        GLuint _time_handle;
        GLuint _res_handle;

    public:
        void render();
        void disableAttributes();

        /* Ctor - Init Shaders */
        BackgroundRenderer();
};

#endif /* BACKGROUND_RENDERER_H */
