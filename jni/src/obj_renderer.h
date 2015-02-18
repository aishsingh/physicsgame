#ifndef OBJ_RENDERER_H
#define OBJ_RENDERER_H

#include <GLES2/gl2.h>
#include "renderer.h"
#include "camera.h"
#include "colour.h"

class ObjRenderer: public Renderer {
    private:
        GLuint _vColor_handle;
        Camera *_cam;

    public:
        void render(vector<float> vertices, Colour colour, float angle, GLenum mode);
        void disableAttributes();

        /* Ctor - Init Shaders */
        ObjRenderer(Camera *cam);
};

#endif /* OBJ_RENDERER_H */
