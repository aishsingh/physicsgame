#ifndef OBJ_RENDERER_H
#define OBJ_RENDERER_H

#include <GLES2/gl2.h>
#include "renderer.h"
#include "camera.h"

class ObjRenderer: public Renderer {
    private:
        GLuint _gvColorHandle;
        void render(vector<float> vertices, vector<float> colours, float angle, GLenum mode);

    public:
        void render(vector<float> vertices, vector<float> colours, float angle, GLenum mode, Camera *cam);
        void disableAttributes();

        /* Ctor - Init Shaders */
        ObjRenderer();
};

#endif /* OBJ_RENDERER_H */
