#include <stdlib.h>
#include <math.h>
#include "screen_renderer.h"
#include "log.h"
#include "game.h"

ScreenRenderer::ScreenRenderer() {
    _shad_vertex =
        "attribute vec2 vPos;\n"
        "attribute vec4 vColor;\n"
        "varying vec4 vFragColor;\n"
        "uniform mat4 mProj;\n"

        "void main() {\n"
        "  gl_Position = mProj * vec4(vPos, 0.0f, 1.0f);\n"

        "  vFragColor = vColor;\n"
        "}\n";

    _shad_fragment = 
        "precision mediump float;\n"
        "varying vec4 vFragColor;\n"

        "void main() {\n"
        "  gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
        "}\n";

    int screen_w = Game::getScreenWidth();
    int screen_h = Game::getScreenHeight();
    _gProgram = createProgram(_shad_vertex.c_str(), _shad_fragment.c_str());
    if (!_gProgram) {
        LOGE("Could not create program.");
    }

    _gvPosHandle = glGetAttribLocation(_gProgram, "vPos");
    checkGlError("glGetAttribLocation(vPos)");

    _gvColorHandle = glGetAttribLocation(_gProgram, "vColor");
    checkGlError("glGetAttribLocation(vColor)");

    GLuint gmProjHandle = glGetUniformLocation(_gProgram, "mProj");
    checkGlError("glGetUniformLocation(mProj)");
 
    /* Projection Matrix */
    GLfloat proj[] = { 2.0f/screen_w, 0.0f,          0.0f, 0.0f,
                       0.0f,         -2.0f/screen_h, 0.0f, 0.0f,
                       0.0f,          0.0f,          0.0f, 0.0f,
                      -1.0f,          1.0f,          0.0f, 1.0f };

    // Pass uniforms to shader
    /* VERY IMPORTANT
     * glUseProgram() needs to be called before you setup a uniform 
     * but not needed before glGetUniformLocation() 
     * http://www.opengl.org/wiki/GLSL_:_common_mistakes */
    glUseProgram(_gProgram);
    checkGlError("glUseProgram");

    glUniformMatrix4fv(gmProjHandle, 1, GL_FALSE, &proj[0]);
    checkGlError("glUniformMatrix4fv, mProj");

    glViewport(0, 0, screen_w, screen_h);
    checkGlError("glViewport");

    glDisable(GL_DEPTH_TEST);
    checkGlError("glDisable(GL_DEPTH_TEST)");
}

void ScreenRenderer::render(vector<float> vertices, vector<float> colours, float angle, GLenum mode) {
    // Change renderer
    glUseProgram(_gProgram);
    checkGlError("glUseProgram");

    glVertexAttribPointer(_gvPosHandle, 2, GL_FLOAT, GL_FALSE, 0, &vertices[0]);
    glVertexAttribPointer(_gvColorHandle, 4, GL_FLOAT, GL_FALSE, 0, &colours[0]);
    checkGlError("glVertexAttribPointer");

    glEnableVertexAttribArray(_gvPosHandle);
    glEnableVertexAttribArray(_gvColorHandle);
    checkGlError("glEnableVertexAttribArray");

    glDrawArrays(mode, 0, vertices.size()/2);
    checkGlError("ui glDrawArrays");

    // Can be called directly by the renderer as this will be the only UI renderer instance
    disableAttributes();
}

void ScreenRenderer::disableAttributes() {
    // Any unique attributes in these shaders needs to be disabled here
    glDisableVertexAttribArray(_gvPosHandle);
    glDisableVertexAttribArray(_gvColorHandle);
}
