#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <vector>
#include <GLES2/gl2.h>
#include <png.h>
#include "zip.h"
#include "colour.h"
#include "shape.h"

class Renderer {
    protected:
        GLuint gProgram;
        GLuint gvPosHandle;
        GLuint gfAngleHandle;
        GLuint gvColorHandle;

        std::string shad_vertex;
        std::string shad_fragment;

        GLuint loadShader(GLenum shaderType, const char *pSource);
        GLuint createProgram(const char *pVertexSource, const char *pFragmentSource);

        /* Functions called from renderObject() */
        virtual std::vector<float> useColour(Colour *colour); 
        virtual std::vector<float> useObjectVertices(Object *obj) = 0;
        virtual void setShaderData(float vertices[], float colours[], float angle) = 0;

    public:
        virtual bool setup() = 0;

        /* Send vectors and other data to shaders */
        virtual void renderShape(Shape *obj);
        
        virtual void disableAttributes() = 0;

        static void clearScreen();

        /* Ctor - Init Shaders */
        Renderer();
        virtual ~Renderer();
};

#endif /* RENDERER_H */
