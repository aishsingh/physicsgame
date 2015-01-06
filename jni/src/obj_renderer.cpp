#include <stdlib.h>
#include "obj_renderer.h"
#include "log.h"
#include "game.h"

ObjRenderer::ObjRenderer() {
    _shad_vertex =
        "attribute vec2 vPos;\n"
        "attribute vec4 vColor;\n"
        "attribute float fAngle;\n"
        "varying vec4 vFragColor;\n"
        "uniform mat4 mProj;\n"

        "void main() {\n"
        "  vec2 pos = vPos;\n"
        "  pos.x = vPos.x*cos(fAngle) - vPos.y*sin(fAngle);\n"
        "  pos.y = vPos.y*cos(fAngle) + vPos.x*sin(fAngle);\n"

        "  mat4 mMVP = mProj;\n"
        "  gl_Position = mMVP * vec4(pos, 0.0f, 1.0f);\n"

        "  vFragColor = vColor;\n"
        "}\n";

    _shad_fragment = 
        "precision mediump float;\n"
        "varying vec4 vFragColor;\n"

        "void main() {\n"
        "  gl_FragColor = vFragColor;\n"
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

    _gfAngleHandle = glGetAttribLocation(_gProgram, "fAngle");
    checkGlError("glGetAttribLocation(fAngle)");

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

void ObjRenderer::render(vector<float> vertices, vector<float> colours, float angle, GLenum mode) {
    // Change renderer
    glUseProgram(_gProgram);
    checkGlError("glUseProgram");

    glVertexAttribPointer(_gvPosHandle, 2, GL_FLOAT, GL_FALSE, 0, &vertices[0]);
    glVertexAttribPointer(_gvColorHandle, 4, GL_FLOAT, GL_FALSE, 0, &colours[0]);
    glVertexAttrib1f(_gfAngleHandle, angle);
    checkGlError("glVertexAttrib");

    glEnableVertexAttribArray(_gvPosHandle);
    glEnableVertexAttribArray(_gvColorHandle);
    checkGlError("glEnableVertexAttribArray");

    // Pass values to shader
    glDrawArrays(mode, 0, vertices.size()/2);
    checkGlError("box glDrawArrays");
}

void ObjRenderer::disableAttributes() {
    glDisableVertexAttribArray(_gvPosHandle);
    glDisableVertexAttribArray(_gvColorHandle);
}
