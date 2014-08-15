#ifndef RENDERER_H
#define RENDERER_H

#include <GLES2/gl2.h>
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

    static const char gVertexShader[];
    static const char gFragmentShader[];

    GLuint loadShader(GLenum shaderType, const char* pSource);
    GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);

public:
    bool setupGraphics(int w, int h);
    void renderFrame();
    void renderBox(Box &box);
};

#endif /* RENDERER_H */
