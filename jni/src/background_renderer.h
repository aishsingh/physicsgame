#ifndef BACKGROUND_RENDERER_H
#define BACKGROUND_RENDERER_H

#include <GLES2/gl2.h>
#include "renderer.h"
#include "camera.h"

class BackgroundRenderer: public Renderer {
    private:
        GLuint _time_handle;
        GLuint _res_handle;

        Camera *_cam;
    public:
        void render();
        void disableAttributes();

        /* Ctor - Init Shaders */
        BackgroundRenderer(Camera *cam);
};

#endif /* BACKGROUND_RENDERER_H */
