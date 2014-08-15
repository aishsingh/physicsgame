#define GLM_FORCE_RADIANS
#include "glm/glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include <math.h>
#include "renderer.h"
#include "log.h"

#define PI 3.14159265358979323846264

const char Renderer::gVertexShader[] = 
    "attribute vec2 vPos;\n"
    "attribute vec4 vColor;\n"
    "attribute float fAngle;\n"
    "varying vec4 vFragColor;\n"
    "uniform mat4 mProj;\n"
    "uniform mat4 mModel;\n"


    "void main() {\n"

    "  float PI = 3.14159265358979323846264;\n"
    // "  float angle = 45.0;\n"
    "  float rad_angle = fAngle*PI/180.0;\n"

    "  vec2 pos = vPos;\n"
    "  pos.x = vPos.x*cos(rad_angle) - vPos.y*sin(rad_angle);\n"
    "  pos.y = vPos.y*cos(rad_angle) + vPos.x*sin(rad_angle);\n"
    "  mat4 mMVP = mProj * mModel;\n"
    "  gl_Position = mMVP * vec4(pos, 0.0f, 1.0f);\n"

    "  vFragColor = vColor;\n"
    "}\n";

const char Renderer::gFragmentShader[] = 
    "precision mediump float;\n"
    "varying vec4 vFragColor;\n"

    "void main() {\n"
    "  gl_FragColor = vFragColor;\n"
    "}\n";

GLuint Renderer::loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d:\n%s\n",
                            shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}


GLuint Renderer::createProgram(const char* pVertexSource, const char* pFragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader) {
        return 0;
    }

    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader) {
        return 0;
    }

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        checkGlError("glAttachShader");
        glAttachShader(program, pixelShader);
        checkGlError("glAttachShader");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGE("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

bool Renderer::setupGraphics(int w, int h) {
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    LOGI("setupGraphics(%d, %d)", w, h);
    gProgram = createProgram(gVertexShader, gFragmentShader);
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

    // gmTranslateHandle = glGetUniformLocation(gProgram, "mTranslate");
    // checkGlError("glGetUniformLocation(mTranslate)");

    // gmViewHandle = glGetUniformLocation(gProgram, "mView");
    // checkGlError("glGetUniformLocation(mView)");

    /* Projection Matrix */
    GLfloat proj[] = { 2.0f/w, 0.0f,   0.0f, 0.0f,
                       0.0f,  -2.0f/h, 0.0f, 0.0f,
                       0.0f,   0.0f,   0.0f, 0.0f,
                      -1.0f,   1.0f,   0.0f, 1.0f };

    /* View Matrix */
    // GLfloat view[] = { 1.0f };

    /* Model Matrix */
    float x = 45.0f;
    float cosx = cos(x * (PI/180.0f));
    float sinx = cos(x * (PI/180.0f));

    GLfloat rotate[]    = { cosx,-sinx, 0.0f, 0.0f, 
                            sinx, cosx, 0.0f, 0.0f, 
                            0.0f, 0.0f, 1.0f, 0.0f, 
                            0.0f, 0.0f, 0.0f, 1.0f };

    // GLfloat rotate[]    = { 1.0f, 0.0f, 0.0f, 0.0f, 
    //                         0.0f, cosx, sinx, 0.0f, 
    //                         0.0f,-sinx, cosx, 0.0f, 
    //                         0.0f, 0.0f, 0.0f, 1.0f };

    // glm::mat4 model = glm::lookAt(glm::vec3(0.0f, 15.0f,40.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));


    GLfloat model[] =     { 1.0f, 0.0f, 0.0f, 0.0f, 
                            0.0f, 1.0f, 0.0f, 0.0f, 
                            0.0f, 0.0f, 1.0f, 0.0f, 
                            0.0f, 0.0f, 0.0f, 1.0f };


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

    // glUniformMatrix4fv(gmTranslateHandle, 1, GL_FALSE, &translate[0]);
    // checkGlError("glUniformMatrix4fv, mTranslate");

    // glUniformMatrix4fv(gmViewHandle, 1, GL_FALSE, &view[0]);
    // checkGlError("glUniformMatrix4fv, mView");

    glViewport(0, 0, w, h);
    checkGlError("glViewport");

    glDisable(GL_DEPTH_TEST);
    return true;
}

/*
void drawBox(GLuint posHandle, GLfloat &aspect) {
    static const GLfloat triVertices[]    = {  0.0f,  0.0f,
                                               0.0f,  aspect * 0.1f,  
                                               0.1f,  0.0f,        
                                               0.1f,  aspect * 0.1f,
                                               0.0f,  aspect * 0.1f, 
                                               0.1f,  0.0f          }; 


    // const GLfloat objVertices[]           = {  obj.getX(),                   obj.getY(),
    //                                            obj.getX(),                   aspect * (obj.getY() + obj.getHeight()),
    //                                           (obj.getX() + obj.getWidth()), obj.getY(),
    //                                           (obj.getX() + obj.getWidth()), aspect * (obj.getY() + obj.getHeight()),
    //                                            obj.getX(),                   aspect * (obj.getY() + obj.getHeight()),
    //                                           (obj.getX() + obj.getWidth()), obj.getY()                               };

    glVertexAttribPointer(posHandle, 2, GL_FLOAT, GL_FALSE, 0, triVertices);
    checkGlError("glVertexAttribPointer");
    glEnableVertexAttribArray(posHandle);
    checkGlError("glEnableVertexAttribArray");
    glDrawArrays(GL_TRIANGLES, 0, 6);
    checkGlError("glDrawArrays");
}
*/

void Renderer::renderFrame() {
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

void Renderer::renderBox(Box &box) {

    /*  [p2]---[p4]   
         |       |  
         | (box) |
         |       |
        [p1]---[p3]  */

    /* This is the original (x,y) that will now be transformed
     * before being passed to the vertex shader */
    float x = box.getX();
    float y = box.getY();

    // Translate to center
    x += (box.getWidth()/2);
    y += (box.getHeight()/2);

    // Rotate
    static float angle = 0;
    if (angle >= 360.0f)
        angle = 0.0f;
    else
        angle += 0.1f;

    float rad_angle = angle*PI/180.0;
    float rot_x = x*cos(rad_angle) + y*sin(rad_angle);
    float rot_y = y*cos(rad_angle) - x*sin(rad_angle);

    x = rot_x;
    y = rot_y;

    // Translate back to origin
    x -= (box.getWidth()/2);
    y -= (box.getHeight()/2);
    

    // Declare points (x,y)
    float p1[2] = { x                 , y                   };
    float p2[2] = { x                 , y + box.getHeight() };
    float p3[2] = { x + box.getWidth(), y                   };
    float p4[2] = { x + box.getWidth(), y + box.getHeight() };

    GLfloat boxVertices[] = { p1[0], p1[1],
                              p2[0], p2[1],
                              p3[0], p3[1],
                              p4[0], p4[1] };

    // Declare colors (r,g,b,a)
    Colour colour = box.getColour();
    GLfloat boxColour[] = { colour.r, colour.g, colour.b, colour.a,
                            colour.r, colour.g, colour.b, colour.a,
                            colour.r, colour.g, colour.b, colour.a,
                            colour.r, colour.g, colour.b, colour.a };

    // Pass values to shader
    glVertexAttribPointer(gvPosHandle, 2, GL_FLOAT, GL_FALSE, 0, boxVertices);
    glVertexAttribPointer(gvColorHandle, 4, GL_FLOAT, GL_FALSE, 0, boxColour);
    checkGlError("glVertexAttribPointer");
    glVertexAttrib1f(gfAngleHandle, angle);
    checkGlError("glVertexAttrib");

    glEnableVertexAttribArray(gvPosHandle);
    glEnableVertexAttribArray(gvColorHandle);
    checkGlError("glEnableVertexAttribArray");

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    checkGlError("glDrawArrays");
}
