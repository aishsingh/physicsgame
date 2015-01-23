#include <stdlib.h>
#include <cmath>
#include "screen_renderer.h"
#include "log.h"
#include "game.h"

ScreenRenderer::ScreenRenderer() {
    _shad_vertex =
        "attribute vec2 vPos;\n"
        "attribute vec4 vColor;\n"
        "varying vec4 vFragColor;\n"
        "uniform mat4 mMVP;\n"

        "void main() {\n"
        "  gl_Position = mMVP * vec4(vPos, 0.0f, 1.0f);\n"

        "  vFragColor = vColor;\n"
        "}\n";

    _shad_fragment = 
        "precision mediump float;\n"
        "varying vec4 vFragColor;\n"

        "void main() {\n"
        "  gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
        "}\n";

    _program = createProgram(_shad_vertex.c_str(), _shad_fragment.c_str());
    if (!_program) {
        LOGE("Could not create program.");
    }

    _vPos_handle = glGetAttribLocation(_program, "vPos");
    checkGlError("glGetAttribLocation(vPos)");

    _vColor_handle = glGetAttribLocation(_program, "vColor");
    checkGlError("glGetAttribLocation(vColor)");

    _mMVP_handle = glGetUniformLocation(_program, "mMVP");
    checkGlError("glGetUniformLocation(mMVP)");

    /* glUseProgram() needs to be called before you setup a uniform 
     * but not needed before glGetUniformLocation() 
     * http://www.opengl.org/wiki/GLSL_:_common_mistakes */
    glUseProgram(_program);
    checkGlError("glUseProgram");

    // Pass MVP to shader
    glUniformMatrix4fv(_mMVP_handle, 1, GL_FALSE, glm::value_ptr(_proj_mat));
    checkGlError("glUniformMatrix4fv, mMVP");
}

void ScreenRenderer::render(vector<float> vertices, vector<float> colours, float angle, GLenum mode) {
    // Change renderer
    glUseProgram(_program);
    
    glVertexAttribPointer(_vPos_handle, 2, GL_FLOAT, GL_FALSE, 0, &vertices[0]);
    glVertexAttribPointer(_vColor_handle, 4, GL_FLOAT, GL_FALSE, 0, &colours[0]);
    checkGlError("glVertexAttribPointer");

    glEnableVertexAttribArray(_vPos_handle);
    glEnableVertexAttribArray(_vColor_handle);
    checkGlError("glEnableVertexAttribArray");

    // Pass attributes to shader
    glDrawArrays(mode, 0, vertices.size()/2);
    checkGlError("glDrawArrays");

    // Can be called directly by the renderer as this will be the only UI renderer instance
    disableAttributes();
}

void ScreenRenderer::disableAttributes() {
    // Any unique attributes in these shaders needs to be disabled here
    glDisableVertexAttribArray(_vPos_handle);
    glDisableVertexAttribArray(_vColor_handle);
}
