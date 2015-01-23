#ifndef ASSET_RENDERER_H
#define ASSET_RENDERER_H

#include <GLES2/gl2.h>
#include "renderer.h"
#include "camera.h"

class AssetRenderer: public Renderer {
    private:
        GLuint _vTexCoord_handle;
        GLint _sTexture_handle;

        Camera *_cam;

    public:
        void render(vector<float> vertices, vector<float> tex_vertices, GLuint texture_id, float angle, GLenum mode);
        void disableAttributes();

        /* Ctor - Init Shaders */
        AssetRenderer(Camera *cam);
};

#endif /* ASSET_RENDERER_H */
