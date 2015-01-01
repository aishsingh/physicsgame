#ifndef OBJ_RENDERER_H
#define OBJ_RENDERER_H

#include <GLES2/gl2.h>
#include "renderer.h"

class ObjRenderer: public Renderer {
    private:
        GLuint _gfAngleHandle;
        GLuint _gvColorHandle;

    public:
        void render(vector<float> vertices, vector<float> colours, float angle, GLenum mode);
        void disableAttributes();

        /* Ctor - Init Shaders */
        ObjRenderer();
};

#endif /* OBJ_RENDERER_H */
