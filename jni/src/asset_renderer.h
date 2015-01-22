#ifndef ASSET_RENDERER_H
#define ASSET_RENDERER_H

#include <GLES2/gl2.h>
#include "renderer.h"
#include "camera.h"

class AssetRenderer: public Renderer {
    private:
        GLuint _texture_id;
        GLuint _texcoord;
        GLint _uniform_mytexture;

        Camera *_cam;

        // PNG loading
        static void png_zip_read(png_structp png_ptr, png_bytep data, png_size_t length);
        GLuint getBytesFromPNG(const char* filename, zip *APKArchive, int &width, int &height);

        // Texture loading
        GLuint tex;
        GLuint createSimpleTexture2D(GLuint _textureid, GLubyte* pixels, int width, int height, int channels);

    public:
        void render(vector<float> vertices, vector<float> colours, float angle, GLenum mode);
        void disableAttributes();

        /* Ctor - Init Shaders */
        AssetRenderer(Camera *cam, zip *apk);
};

#endif /* ASSET_RENDERER_H */
