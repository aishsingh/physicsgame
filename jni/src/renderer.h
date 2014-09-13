#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <GLES2/gl2.h>
#include <png.h>
#include "zip.h"
#include "shapes.h"

using std::string;

class Renderer {
private:
    float aspect;
    GLuint gProgram;
    GLuint gvPosHandle;
    GLuint gfAngleHandle;
    GLuint gvColorHandle;
    GLuint gmProjHandle;
    GLuint gmModelHandle;
    GLuint gsTexHandle;

    string shad_vertex;
    string shad_fragment;

    GLuint loadShader(GLenum shaderType, const char* pSource);
    GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);

    // PNG loading
    static void png_zip_read(png_structp png_ptr, png_bytep data, png_size_t length);
    GLubyte* getBytesFromPNG(const char* filename, zip *APKArchive, int &width, int &height);

    // Texture loading
    GLuint tex;
    GLuint createSimpleTexture2D(GLuint _textureid, GLubyte* pixels, int width, int height, int channels);
public:

    // bool setup(int w, int h, zip *APKArchive);
    bool setup(int screen_w, int screen_h);
    void renderFrame();
    void renderBox(Box &box);

    /* Ctor - Init Shaders */
    Renderer();
};

#endif /* RENDERER_H */
