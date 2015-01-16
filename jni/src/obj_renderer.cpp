#include <stdlib.h>
#include "obj_renderer.h"
#include "log.h"
#include "game.h"

ObjRenderer::ObjRenderer() {
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
        "  gl_FragColor = vFragColor;\n"
        "}\n";

    _gProgram = createProgram(_shad_vertex.c_str(), _shad_fragment.c_str());
    if (!_gProgram) {
        LOGE("Could not create program.");
    }

    _gvPosHandle = glGetAttribLocation(_gProgram, "vPos");
    checkGlError("glGetAttribLocation(vPos)");

    _gvColorHandle = glGetAttribLocation(_gProgram, "vColor");
    checkGlError("glGetAttribLocation(vColor)");

    _gmMVPHandle = glGetUniformLocation(_gProgram, "mMVP");
    checkGlError("glGetUniformLocation(mMVP)");
}

void ObjRenderer::render(vector<float> vertices, vector<float> colours, float angle, GLenum mode) {
    glVertexAttribPointer(_gvPosHandle, 2, GL_FLOAT, GL_FALSE, 0, &vertices[0]);
    glVertexAttribPointer(_gvColorHandle, 4, GL_FLOAT, GL_FALSE, 0, &colours[0]);
    checkGlError("glVertexAttrib");

    glEnableVertexAttribArray(_gvPosHandle);
    glEnableVertexAttribArray(_gvColorHandle);
    checkGlError("glEnableVertexAttribArray");

    // Pass attributes to shader
    glDrawArrays(mode, 0, vertices.size()/2);
    checkGlError("box glDrawArrays");
}

void ObjRenderer::render(vector<float> vertices, vector<float> colours, float angle, GLenum mode, Camera *cam) {
    // Change renderer
    glUseProgram(_gProgram);
    checkGlError("glUseProgram");

    // Model matrix
    glm::mat4 model_mat;
    model_mat= glm::rotate(model_mat, 
                           static_cast<float>(angle*PI/180), 
                           glm::vec3(0.0f, 0.0f, 1.0f));

    // View matrix
    Point2D ctr = Point2D(Game::getScreenWidth()/2, Game::getScreenHeight()/2);
    Point2D anchor_pt = cam->getPos();
    glm::mat4 view_mat;
    view_mat = glm::translate(view_mat, glm::vec3(ctr.getX() - anchor_pt.getX(), ctr.getY() - anchor_pt.getY(), 0));
    if (CAM_RotateViewToFollowPlayer) {
        view_mat = glm::translate(view_mat, glm::vec3(anchor_pt.getX(), anchor_pt.getY(), 0));
        view_mat = glm::rotate(view_mat, 
                               static_cast<float>(cam->getRotAngle()*PI/180), 
                               glm::vec3(0.0f, 0.0f, 1.0f));
        view_mat = glm::translate(view_mat, glm::vec3(-anchor_pt.getX(), -anchor_pt.getY(), 0));
    }

    // MVP
    glm::mat4 MVP_mat = _proj_mat * view_mat * model_mat;

    // Pass MVP to shader
    glUniformMatrix4fv(_gmMVPHandle, 1, GL_FALSE, glm::value_ptr(MVP_mat));
    checkGlError("glUniformMatrix4fv, mMVP");
    render(vertices, colours, angle, mode);
}

void ObjRenderer::disableAttributes() {
    glDisableVertexAttribArray(_gvPosHandle);
    glDisableVertexAttribArray(_gvColorHandle);
}
