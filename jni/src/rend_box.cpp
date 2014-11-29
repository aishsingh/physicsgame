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

#define PI 3.14159265358979323846264
#define TEXTURE_LOAD_ERROR 0


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


bool Rend_box::setup(int screen_w, int screen_h) {
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

    gmProjHandle = glGetUniformLocation(gProgram, "mProj");
    checkGlError("glGetUniformLocation(mProj)");

    gmModelHandle = glGetUniformLocation(gProgram, "mModel");
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

/*
void Rend_box::renderFrame() {
    float bg = 0.0f;
    glClearColor(bg, bg, bg, 1.0f);
    checkGlError("glClearColor");
    glClear(GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");

    glUseProgram(gProgram);
    checkGlError("glUseProgram");

    // Enable Transparancy
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
*/

// void Rend_box::renderObject(Object *obj) {
//     /*  [p2]---[p4]   
//          |       |  
//          | (box) |
//          |       |
//         [p1]---[p3]  */
//
//     /* This is the original (x,y) that will now be transformed
//      * before being passed to the vertex shader */
//     float x = obj.getX();
//     float y = obj.getY();
//
//     // Translate to center
//     x += (obj.getWidth()/2);
//     y += (obj.getHeight()/2);
//
//     // Rotate
//     // static float angle = 0;
//     float angle = obj.rot_angle;
//
//     float rad_angle = angle*PI/180.0;
//     float rot_x =  x*cos(rad_angle) + y*sin(rad_angle);
//     float rot_y = -x*sin(rad_angle) + y*cos(rad_angle);
//
//     x = rot_x;
//     y = rot_y;
//
//     // Translate back to origin
//     x -= (obj.getWidth()/2);
//     y -= (obj.getHeight()/2);
//     
//     // Declare points (x,y)
//     float p1[2] = { x                  , y                    };
//     float p2[2] = { x                  , y + obj.getHeight() };
//     float p3[2] = { x + obj.getWidth(), y                    };
//     float p4[2] = { x + obj.getWidth(), y + obj.getHeight() };
//
//     GLfloat objVertices[] = { p1[0], p1[1],
//                               p2[0], p2[1],
//                               p3[0], p3[1],
//                               p4[0], p4[1] };
//
//     // Declare colors (r,g,b,a)
//     // Colour colour = Colour(0.0f, 0.0f, 0.5f, 1.0f);
//     // Box* box = dynamic_cast<Box*>(obj);
//     Colour colour = static_cast<Shape>(obj).getColour();
//     // Colour colour = dynamic_cast<Box*>(obj).getColour();
//     GLfloat objColour[] = { colour.r, colour.g, colour.b, colour.a,
//                             colour.r, colour.g, colour.b, colour.a,
//                             colour.r, colour.g, colour.b, colour.a,
//                             colour.r, colour.g, colour.b, colour.a };
//
//     // Pass values to shader
//     glVertexAttribPointer(gvPosHandle, 2, GL_FLOAT, GL_FALSE, 0, objVertices);
//     glVertexAttribPointer(gvColorHandle, 4, GL_FLOAT, GL_FALSE, 0, objColour);
//     checkGlError("glVertexAttribPointer");
//     glVertexAttrib1f(gfAngleHandle, angle);
//     checkGlError("glVertexAttrib");
//
//     glEnableVertexAttribArray(gvPosHandle);
//     glEnableVertexAttribArray(gvColorHandle);
//     checkGlError("glEnableVertexAttribArray");
//
//     glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//     checkGlError("glDrawArrays");
// }

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
}

std::vector<float> Rend_box::useColour(Colour *colour) {

    float clr[] = { colour->r, colour->g, colour->b, colour->a,
                    colour->r, colour->g, colour->b, colour->a,
                    colour->r, colour->g, colour->b, colour->a,
                    colour->r, colour->g, colour->b, colour->a };

    return std::vector<float> (clr, clr + sizeof clr / sizeof clr[0]);
}

std::vector<float> Rend_box::useObjectVertices(Object *obj) {
    /*  [p2]---[p4]   
         |       |  
         | (box) |
         |       |
        [p1]---[p3]  */


    /* This is the original (x,y) that will now be transformed
     * before being passed to the vertex shader */
    float x = obj->getX();
    float y = obj->getY();

    // Translate to center
    x += (obj->getWidth()/2);
    y += (obj->getHeight()/2);

    // Rotate
    // static float angle = 0;
    float objAngle = obj->rot_angle;

    float rad_angle = objAngle*PI/180.0;
    float rot_x =  x*cos(rad_angle) + y*sin(rad_angle);
    float rot_y = -x*sin(rad_angle) + y*cos(rad_angle);

    x = rot_x;
    y = rot_y;

    // Translate back to origin
    x -= (obj->getWidth()/2);
    y -= (obj->getHeight()/2);
    
    // Declare points (x,y)
    float p1[2] = { x                  , y                    };
    float p2[2] = { x                  , y + obj->getHeight() };
    float p3[2] = { x + obj->getWidth(), y                    };
    float p4[2] = { x + obj->getWidth(), y + obj->getHeight() };

    float vert[] = { p1[0], p1[1],
                     p2[0], p2[1],
                     p3[0], p3[1],
                     p4[0], p4[1] };

    return std::vector<float> (vert, vert + sizeof vert / sizeof vert[0]);
}

void Rend_box::setShaderData(float vertices[], float colours[], float angle) {
    glVertexAttribPointer(gvPosHandle, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(gvColorHandle, 4, GL_FLOAT, GL_FALSE, 0, colours);
    checkGlError("glVertexAttribPointer");
    glVertexAttrib1f(gfAngleHandle, angle);
    checkGlError("glVertexAttrib");

    glEnableVertexAttribArray(gvPosHandle);
    glEnableVertexAttribArray(gvColorHandle);
    checkGlError("glEnableVertexAttribArray");

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    checkGlError("glDrawArrays");
}
