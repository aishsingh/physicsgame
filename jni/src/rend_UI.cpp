/* GLM lib
  #define GLM_FORCE_RADIANS
  #include "glm/glm/glm.hpp"
  #include <glm/gtc/matrix_transform.hpp>
  #include <glm/gtc/type_ptr.hpp>
*/

#include <stdlib.h>
#include <math.h>
#include "rend_UI.h"
#include "log.h"
#include "game.h"

Rend_UI::Rend_UI() {
    shad_vertex =
        "attribute vec2 vPos;\n"
        // "attribute vec4 vColor;\n"
        // "varying vec4 vFragColor;\n"
        "uniform mat4 mProj;\n"
        "uniform mat4 mModel;\n"

        "void main() {\n"
        "  mat4 mMP = mProj * mModel;\n"
        "  gl_Position = mMP * vec4(vPos, 0.0f, 1.0f);\n"

        // "  vFragColor = vColor;\n"
        "}\n";

    shad_fragment = 
        "precision mediump float;\n"
        // "varying vec4 vFragColor;\n"

        "void main() {\n"
        "  gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
        "}\n";
}

bool Rend_UI::setup() {
    int screen_w = Game::getScreenWidth();
    int screen_h = Game::getScreenHeight();
    gProgram = createProgram(shad_vertex.c_str(), shad_fragment.c_str());
    if (!gProgram) {
        LOGE("Could not create program.");
        return false;
    }

    gvPosHandle = glGetAttribLocation(gProgram, "vPos");
    checkGlError("glGetAttribLocation(vPos)");

    // gvColorHandle = glGetAttribLocation(gProgram, "vColor");
    // checkGlError("glGetAttribLocation(vColor)");

    GLuint gmProjHandle = glGetUniformLocation(gProgram, "mProj");
    checkGlError("glGetUniformLocation(mProj)");

    GLuint gmModelHandle = glGetUniformLocation(gProgram, "mModel");
    checkGlError("glGetUniformLocation(mModel)");
 
    /* Projection Matrix */
    GLfloat proj[] = { 2.0f/screen_w, 0.0f,          0.0f, 0.0f,
                       0.0f,         -2.0f/screen_h, 0.0f, 0.0f,
                       0.0f,          0.0f,          0.0f, 0.0f,
                      -1.0f,          1.0f,          0.0f, 1.0f };

    /* Model Matrix */
    // Identity Matrix
    GLfloat model[] = { 1.0f, 0.0f, 0.0f, 0.0f, 
                        0.0f, 1.0f, 0.0f, 0.0f, 
                        0.0f, 0.0f, 1.0f, 0.0f, 
                        0.0f, 0.0f, 0.0f, 1.0f };


    // Pass uniforms to shader
    /* VERY IMPORTANT
     * glUseProgram() needs to be called before you setup a uniform 
     * but not needed before glGetUniformLocation() 
     * http://www.opengl.org/wiki/GLSL_:_common_mistakes */
    glUseProgram(gProgram);
    checkGlError("glUseProgram");

    glUniformMatrix4fv(gmProjHandle, 1, GL_FALSE, &proj[0]);
    checkGlError("glUniformMatrix4fv, mProj");

    glUniformMatrix4fv(gmModelHandle, 1, GL_FALSE, &model[0]);
    checkGlError("glUniformMatrix4fv, mModel");

    glViewport(0, 0, screen_w, screen_h);
    checkGlError("glViewport");

    glDisable(GL_DEPTH_TEST);
    checkGlError("glDisable(GL_DEPTH_TEST)");
    LOGI("--------------------");
    return true;
}

void Rend_UI::disableAttributes() {
    // Any unique attributes in these shaders needs to be disabled here
    glDisableVertexAttribArray(gvPosHandle);
}

void Rend_UI::renderUI() {
    // Joystick controls player rotation
    Object joystick1 = Object(150, Game::getScreenHeight() - 200, 200, 200);
    
    // Positioning
    std::vector<float> js1Vertices = useObjectVertices(&joystick1);

    // Pass values to shader
    setShaderData(&js1Vertices[0], 0, 0);

    // Can be called directly by the renderer as this will be the only UI renderer instance
    disableAttributes();
}

std::vector<float> Rend_UI::useObjectVertices(Object *obj) {
    /*     [p3] [p4]          Origin Cross     
              ---             
        [p1]       [p5]          [p11]
           |   +   |     ---> [p9] + [p10]
        [p2]       [p6]          [p12]
              ---            
           [p8] [p7]                      */


    /* This is the original (x,y) that will now be transformed
     * before being passed to the vertex shader */
    float x = obj->getX();
    float y = obj->getY();
    float l = obj->getWidth();
    float size = 60.0f;
    float offset = (l - size)/2;

    // Orgin points
    float ori_size = 10.0f;
    
    // Declare points (x,y)
    float vec[] = { x                        , y + offset               ,
                    x                        , y + size + offset        ,
                    x + offset               , y                        ,
                    x + size + offset        , y                        ,
                    x + l                    , y + offset               ,
                    x + l                    , y + size + offset        ,
                    x + offset               , y + l                    ,
                    x + size + offset        , y + l                    ,
                    x + (l/2) - (ori_size/2) , y + (l/2)                ,
                    x + (l/2) + (ori_size/2) , y + (l/2)                ,
                    x + (l/2)                , y + (l/2) - (ori_size/2) ,
                    x + (l/2)                , y + (l/2) + (ori_size/2) };

    return std::vector<float> (vec, vec + sizeof(vec) / sizeof(float));
}

void Rend_UI::setShaderData(float vertices[], float colours[], float angle) {
    // Change renderer
    glUseProgram(gProgram);
    checkGlError("glUseProgram");

    glVertexAttribPointer(gvPosHandle, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    // glVertexAttribPointer(gvColorHandle, 4, GL_FLOAT, GL_FALSE, 0, colours);
    checkGlError("glVertexAttribPointer");

    glEnableVertexAttribArray(gvPosHandle);
    // glEnableVertexAttribArray(gvColorHandle);
    checkGlError("glEnableVertexAttribArray");

    glDrawArrays(GL_LINES, 0, 12);
    checkGlError("ui glDrawArrays");
}
