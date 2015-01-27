#include <stdlib.h>
#include "asset_renderer.h"
#include "log.h"
#include "game.h"

#define PI 3.14159265358979323846264

AssetRenderer::AssetRenderer(Camera *cam) : _cam(cam) {
    _shad_vertex =
        "attribute vec2 vPos;\n"
        "attribute vec2 vTex_coord;\n"
        "varying vec2 f_vTex_coord;\n"
        "uniform mat4 mMVP;\n"

        "void main() {\n"
        "  gl_Position = mMVP * vec4(vPos, 0.0, 1.0);\n"
        "  f_vTex_coord = vTex_coord;\n"
        "}\n";

    _shad_fragment = 
        "precision mediump float;\n"
        "varying vec2 f_vTex_coord;\n"
        "uniform sampler2D sTexture;\n"

        "void main() {\n"
        "  gl_FragColor = texture2D(sTexture, f_vTex_coord);\n"
        "}\n";

    _program = createProgram(_shad_vertex.c_str(), _shad_fragment.c_str());
    if (!_program) {
        LOGE("Could not create program.");
    }
    _vPos_handle = glGetAttribLocation(_program, "vPos");
    checkGlError("glGetAttribLocation(vPos)");

    _vTexCoord_handle = glGetAttribLocation(_program, "vTex_coord");
    checkGlError("glGetAttribLocation(vTex_coord)");

    _sTexture_handle = glGetUniformLocation(_program, "sTexture");
    checkGlError("glGetUniformLocation(sTexture)");

    _mMVP_handle = glGetUniformLocation(_program, "mMVP");
    checkGlError("glGetUniformLocation(mMVP)");
}

void AssetRenderer::render(vector<float> vertices, vector<float> tex_vertices, GLuint texture_id, float angle, GLenum mode) {
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
    view_mat = glm::scale(view_mat, glm::vec3(_cam->getScale(), _cam->getScale(), _cam->getScale()));
    view_mat = glm::translate(view_mat, glm::vec3(-anchor_pt.getX(), -anchor_pt.getY(), 0));

    // MVP
    glm::mat4 MVP_mat = _proj_mat * view_mat * model_mat;

    /* ------------------------------- */

    // Pass MVP to shader
    glUniformMatrix4fv(_mMVP_handle, 1, GL_FALSE, glm::value_ptr(MVP_mat));
    checkGlError("glUniformMatrix4fv, mMVP");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glUniform1i(_sTexture_handle, /*GL_TEXTURE*/0);
    checkGlError("glUniform1i, sTexture");

    glVertexAttribPointer(_vPos_handle, 2, GL_FLOAT, GL_FALSE, 0, &vertices[0]);
    glVertexAttribPointer(_vTexCoord_handle, 2, GL_FLOAT, GL_FALSE, 0, &tex_vertices[0]);
    checkGlError("glVertexAttrib");

    glEnableVertexAttribArray(_vPos_handle);
    glEnableVertexAttribArray(_vTexCoord_handle);
    checkGlError("glEnableVertexAttribArray");

    // Pass attributes to shader
    glDrawArrays(mode, 0, vertices.size()/2);
    checkGlError("glDrawArrays");
}

void AssetRenderer::disableAttributes() {
    glDisableVertexAttribArray(_vPos_handle);
    glDisableVertexAttribArray(_vTexCoord_handle);
}
