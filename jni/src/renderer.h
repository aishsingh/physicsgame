#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <vector>
#include <GLES2/gl2.h>
#include <png.h>
#include "zip.h"
#include "colour.h"

#define PI 3.14159265358979323846264
using std::vector;

class Renderer {
    protected:
        GLuint _gProgram;
        GLuint _gvPosHandle;

        std::string _shad_vertex;
        std::string _shad_fragment;

        GLuint loadShader(GLenum shaderType, const char *pSource);
        GLuint createProgram(const char *pVertexSource, const char *pFragmentSource);

    public:
        virtual bool setup() = 0;
        virtual void render(vector<float> vertices, vector<float> colours, float angle, GLenum mode) = 0;
        virtual void disableAttributes() = 0;

        static void clearScreen();

        /* Ctor / Dtor */
        Renderer();
        virtual ~Renderer();
};

#endif /* RENDERER_H */
