#ifndef OBJ_RENDERER_H
#define OBJ_RENDERER_H

#include <GLES2/gl2.h>
#include "renderer.h"
#include "camera.h"

class ObjRenderer: public Renderer {
    private:
        GLuint _gvColorHandle;
        Camera *_cam;

    public:
        void render(vector<float> vertices, vector<float> colours, float angle, GLenum mode);
        void disableAttributes();

        /* Ctor - Init Shaders */
        ObjRenderer(Camera *cam);
};

#endif /* OBJ_RENDERER_H */
