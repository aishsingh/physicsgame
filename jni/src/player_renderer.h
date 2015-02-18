#ifndef SPACEMAN_RENDERER_H
#define SPACEMAN_RENDERER_H

#include <GLES2/gl2.h>
#include "asset_renderer.h"
#include "camera.h"

class PlayerRenderer: public AssetRenderer {
    private:
        GLuint _vColor_handle;

    public:
        void render(vector<float> vertices, Colour theme_col, vector<float> tex_vertices, GLuint texture_id, float angle, GLenum mode);
        void disableAttributes();

        /* Ctor - Init Shaders */
        PlayerRenderer(Camera *cam);
};

#endif /* SPACEMAN_RENDERER_H */
