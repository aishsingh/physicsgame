#ifndef RENDERER_H
#define RENDERER_H

#include <GLES2/gl2.h>
#include <png.h>
#include "zip.h"
#include "shapes.h"

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

    static const char gVertexShader[];
    static const char gFragmentShader[];

    GLuint loadShader(GLenum shaderType, const char* pSource);
    GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);

    static void png_zip_read(png_structp png_ptr, png_bytep data, png_size_t length);
    GLubyte* getBytesFromPNG(const char* filename, int &width, int &height);

    GLuint tex;

public:
    bool setupGraphics(int w, int h);
    void renderFrame();
    void renderBox(Box &box);

    //---

    void loadAPK(const char *packageName);
    GLuint createSimpleTexture2D(GLuint _textureid, GLubyte* pixels, int width, int height, int channels);
};

#endif /* RENDERER_H */
