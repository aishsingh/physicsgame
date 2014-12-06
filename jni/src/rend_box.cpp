/* GLM lib
  #define GLM_FORCE_RADIANS
  #include "glm/glm/glm.hpp"
  #include <glm/gtc/matrix_transform.hpp>
  #include <glm/gtc/type_ptr.hpp>
*/

#include <stdlib.h>
#include <math.h>
#include "rend_box.h"
#include "log.h"
#include "game.h"

#define PI 3.14159265358979323846264

Rend_box::Rend_box() {
    shad_vertex =
        "attribute vec2 vPos;\n"
        "attribute vec4 vColor;\n"
        "attribute float fAngle;\n"
        "varying vec4 vFragColor;\n"
        "uniform mat4 mProj;\n"
        "uniform mat4 mModel;\n"

        "void main() {\n"
        "  float PI = 3.14159265358979323846264;\n"
        "  float rad_angle = fAngle*PI/180.0;\n"
        "  vec2 pos = vPos;\n"
        "  pos.x = vPos.x*cos(rad_angle) - vPos.y*sin(rad_angle);\n"
        "  pos.y = vPos.y*cos(rad_angle) + vPos.x*sin(rad_angle);\n"

        "  mat4 mMP = mProj * mModel;\n"
        "  gl_Position = mMP * vec4(pos, 0.0f, 1.0f);\n"

        "  vFragColor = vColor;\n"
        "}\n";

    shad_fragment = 
        "precision mediump float;\n"
        "varying vec4 vFragColor;\n"

        "void main() {\n"
        "  gl_FragColor = vFragColor;\n"
        "}\n";
}

bool Rend_box::setup() {
    int screen_w = Game::getScreenWidth();
    int screen_h = Game::getScreenHeight();

    gProgram = createProgram(shad_vertex.c_str(), shad_fragment.c_str());
    if (!gProgram) {
        LOGE("Could not create program.");
        return false;
    }

    gvPosHandle = glGetAttribLocation(gProgram, "vPos");
    checkGlError("glGetAttribLocation(vPos)");

    gvColorHandle = glGetAttribLocation(gProgram, "vColor");
    checkGlError("glGetAttribLocation(vColor)");

    gfAngleHandle = glGetAttribLocation(gProgram, "fAngle");
    checkGlError("glGetAttribLocation(fAngle)");

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


void Rend_box::renderShape(Shape *obj) {
    // Positioning
    std::vector<float> objVertices = useObjectVertices(obj);

    // Colours
    Colour colour = obj->getColour();
    std::vector<float> objColours = useColour(&colour);

    // Rotate
    float objAngle = obj->rot_angle;

    // Pass values to shader
    setShaderData(&objVertices[0], &objColours[0], objAngle);

    // can be called by the renderer as this will be the only UI renderer
    // disableAttributes();
}

std::vector<float> Rend_box::useObjectVertices(Object *obj) {
    /*  [p1]---[p3]   
         |       |  
         | (box) |
         |       |
        [p2]---[p4]  */


    /* This is the original (x,y) that will now be transformed
     * before being passed to the vertex shader */
    float x = obj->getX();
    float y = obj->getY();
    float w = obj->getWidth();
    float h = obj->getHeight();

    // Translate to center
    x += (w/2);
    y += (h/2);

    // Rotate
    float objAngle = obj->rot_angle;

    float rad_angle = objAngle*PI/180.0;
    float rot_x =  x*cos(rad_angle) + y*sin(rad_angle);
    float rot_y = -x*sin(rad_angle) + y*cos(rad_angle);

    x = rot_x;
    y = rot_y;

    // Translate back to origin
    x -= (w/2);
    y -= (h/2);
    
    // Declare points (x,y)
    float vec[] = { x     , y     ,
                    x     , y + h ,
                    x + w , y     ,
                    x + w , y + h };

    return std::vector<float> (vec, vec + sizeof(vec) / sizeof(float));
}

void Rend_box::setShaderData(float vertices[], float colours[], float angle) {
    // Change renderer
    glUseProgram(gProgram);
    checkGlError("glUseProgram");

    glVertexAttribPointer(gvPosHandle, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(gvColorHandle, 4, GL_FLOAT, GL_FALSE, 0, colours);
    glVertexAttrib1f(gfAngleHandle, angle);
    checkGlError("glVertexAttrib");

    glEnableVertexAttribArray(gvPosHandle);
    glEnableVertexAttribArray(gvColorHandle);
    checkGlError("glEnableVertexAttribArray");

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    checkGlError("box glDrawArrays");
}

void Rend_box::disableAttributes() {
    glDisableVertexAttribArray(gvPosHandle);
    glDisableVertexAttribArray(gvColorHandle);
}
