#define GLM_FORCE_RADIANS 1
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>
#include "obj_renderer.h"
#include "log.h"
#include "game.h"

ObjRenderer::ObjRenderer() {
    _shad_vertex =
        "attribute vec2 vPos;\n"
        "attribute vec4 vColor;\n"
        "varying vec4 vFragColor;\n"
        "uniform mat4 mProj;\n"
        "uniform mat4 mView;\n"
        "uniform mat4 mModel;\n"

        "void main() {\n"
        "  mat4 mMVP = mProj * mView * mModel;\n"
        "  gl_Position = mMVP * vec4(vPos, 0.0f, 1.0f);\n"

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

    GLuint gmProjHandle = glGetUniformLocation(_gProgram, "mProj");
    checkGlError("glGetUniformLocation(mProj)");

    _gmModelHandle = glGetUniformLocation(_gProgram, "mModel");
    checkGlError("glGetUniformLocation(mModel)");

    _gmViewHandle = glGetUniformLocation(_gProgram, "mView");
    checkGlError("glGetUniformLocation(mView)");


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
    glVertexAttribPointer(_gvPosHandle, 2, GL_FLOAT, GL_FALSE, 0, &vertices[0]);
    glVertexAttribPointer(_gvColorHandle, 4, GL_FLOAT, GL_FALSE, 0, &colours[0]);
    checkGlError("glVertexAttrib");

    glEnableVertexAttribArray(_gvPosHandle);
    glEnableVertexAttribArray(_gvColorHandle);
    checkGlError("glEnableVertexAttribArray");

    // Pass values to shader
    glDrawArrays(mode, 0, vertices.size()/2);
    checkGlError("box glDrawArrays");
}

void ObjRenderer::render(vector<float> vertices, vector<float> colours, float angle, GLenum mode, Camera *cam) {
    // Change renderer
    glUseProgram(_gProgram);
    checkGlError("glUseProgram");

    // Update Model matrix
    glm::mat4 model_matrix;
    model_matrix = glm::rotate(model_matrix, 
                               static_cast<float>(angle*PI/180), 
                               glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(_gmModelHandle, 1, GL_FALSE, glm::value_ptr(model_matrix));
    checkGlError("glUniformMatrix4fv, mModel");

    // Update View matrix
    float trans_x = Game::getScreenWidth()/2;
    float trans_y = Game::getScreenHeight()/2;
    glm::mat4 view_matrix;
    view_matrix = glm::translate(view_matrix, glm::vec3(trans_x, trans_y, 0));
    view_matrix = glm::rotate(view_matrix, 
                              static_cast<float>(cam->getRotAngle()*PI/180), 
                              glm::vec3(0.0f, 0.0f, 1.0f));
    view_matrix = glm::translate(view_matrix, glm::vec3(-trans_x, -trans_y, 0));
    glUniformMatrix4fv(_gmViewHandle, 1, GL_FALSE, glm::value_ptr(view_matrix));
    checkGlError("glUniformMatrix4fv, mView");
    
    LOGI("angle %.2f", cam->getRotAngle());

    render(vertices, colours, angle, mode);
}

void ObjRenderer::disableAttributes() {
    glDisableVertexAttribArray(_gvPosHandle);
    glDisableVertexAttribArray(_gvColorHandle);
}
