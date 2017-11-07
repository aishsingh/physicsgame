#include <stdlib.h>
#include "obj_renderer.h"
#include "log.h"
#include "game.h"

ObjRenderer::ObjRenderer(Camera *cam) : _cam(cam) {
    _shad_vertex =
        "attribute vec2 vPos;\n"
        "uniform mat4 mMVP;\n"

        "void main() {\n"
        "  gl_Position = mMVP * vec4(vPos, 0.0, 1.0);\n"
        "}\n";

    _shad_fragment = 
        "precision mediump float;\n"
        "uniform vec4 vColor;\n"

        "void main() {\n"
        "  gl_FragColor = vColor;\n"
        "}\n";

    _program = createProgram(_shad_vertex.c_str(), _shad_fragment.c_str());
    if (!_program) {
        LOGE("Could not create program.");
    }

    _vPos_handle = glGetAttribLocation(_program, "vPos");
    checkGlError("glGetAttribLocation(vPos)");

    _vColor_handle = glGetUniformLocation(_program, "vColor");
    checkGlError("glGetUniformLocation(vColor)");

    _mMVP_handle = glGetUniformLocation(_program, "mMVP");
    checkGlError("glGetUniformLocation(mMVP)");
}

void ObjRenderer::render(vector<float> vertices, Colour colour, float angle, GLenum mode, float scale_offset) {
    // Change renderer
    glUseProgram(_program);
    checkGlError("glUseProgram");

    /* Matrix transformations -------- */

    // Model matrix
    glm::mat4 model_mat;
    model_mat = glm::rotate(model_mat, 
                           static_cast<float>(angle*PI/180), 
                           glm::vec3(0.0f, 0.0f, 1.0f));

    // View matrix
    Point2D ctr = Point2D(Game::getScreenWidth()/2, Game::getScreenHeight()/2);
    Point2D anchor_pt = _cam->getPos();
    glm::mat4 view_mat;
    view_mat = glm::translate(view_mat, glm::vec3(ctr.getX(), ctr.getY(), 0));
    view_mat = glm::rotate(view_mat, 
                           static_cast<float>(_cam->getRotAngle()*PI/180), 
                           glm::vec3(0.0f, 0.0f, 1.0f));
    view_mat = glm::scale(view_mat, glm::vec3(_cam->getScale() + scale_offset, _cam->getScale() + scale_offset, _cam->getScale() + scale_offset));
    view_mat = glm::translate(view_mat, glm::vec3(-anchor_pt.getX(), -anchor_pt.getY(), 0));

    // MVP
    glm::mat4 MVP_mat = _proj_mat * view_mat * model_mat;

    /* ------------------------------- */

    // Pass MVP to shader
    glUniformMatrix4fv(_mMVP_handle, 1, GL_FALSE, glm::value_ptr(MVP_mat));
    checkGlError("glUniformMatrix4fv, mMVP");

    float col[4] {
        colour.r,
        colour.g,
        colour.b,
        colour.a
    };
    glUniform4fv(_vColor_handle, 1, col);
    checkGlError("glUniformMatrix4fv, vColour");

    glVertexAttribPointer(_vPos_handle, 2, GL_FLOAT, GL_FALSE, 0, &vertices[0]);
    checkGlError("glVertexAttrib");

    glEnableVertexAttribArray(_vPos_handle);
    checkGlError("glEnableVertexAttribArray");

    // Pass attributes to shader
    glDrawArrays(mode, 0, vertices.size()/2);
    checkGlError("glDrawArrays");
}

void ObjRenderer::disableAttributes() {
    glDisableVertexAttribArray(_vPos_handle);
}

Camera* ObjRenderer::getCam() const {
    return _cam;
}
