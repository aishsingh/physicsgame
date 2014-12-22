#ifndef ASSET_RENDERER_H
#define ASSET_RENDERER_H

#include <GLES2/gl2.h>
#include "renderer.h"

class AssetRenderer: public Renderer {
    private:
        GLuint _gsTexHandle;
        GLuint _gfAngleHandle;
        GLuint _gvColorHandle;

        // PNG loading
        static void png_zip_read(png_structp png_ptr, png_bytep data, png_size_t length);
        GLubyte* getBytesFromPNG(const char* filename, zip *APKArchive, int &width, int &height);

        // Texture loading
        GLuint tex;
        GLuint createSimpleTexture2D(GLuint _textureid, GLubyte* pixels, int width, int height, int channels);

    public:
        bool setup();
        void render(vector<float> vertices, vector<float> colours, float angle, GLenum mode);
        void disableAttributes();

        /* Ctor - Init Shaders */
        AssetRenderer();
};

#endif /* ASSET_RENDERER_H */