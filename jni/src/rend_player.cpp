#include <stdlib.h>
#include <math.h>
#include "rend_player.h"
#include "log.h"
#include "game.h"

#define PI 3.14159265358979323846264
#define TEXTURE_LOAD_ERROR 0


Rend_player::Rend_player() {
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
/*
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
        "uniform sampler2D sTexture;\n"
        "varying vec4 vFragColor;\n"

        "void main() {\n"
        "  vec2 texCoord = vec2(0.0f, 0.0f);\n"
        "  vec4 baseTex = texture2D(sTexture, texCoord);\n"
        "  vec4 lightCol = vFragColor;\n"
        // "  gl_FragColor = baseTex * (lightCol + 0.25);\n"
        "  gl_FragColor = lightCol;\n"
        "}\n";
*/

bool Rend_player::setup() {
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

    gsTexHandle = glGetUniformLocation(gProgram, "sTexture");
    checkGlError("glGetUniformLocation(sTexture)");

    /* Projection Matrix */
    GLfloat proj[] = { 2.0f/screen_w, 0.0f,          0.0f, 0.0f,
                       0.0f,         -2.0f/screen_h, 0.0f, 0.0f,
                       0.0f,          0.0f,          0.0f, 0.0f,
                      -1.0f,          1.0f,          0.0f, 1.0f };

    /* Model Matrix (Identity Matrix) */
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
    //------------------------------------------------------------------

    /* Enum GL_TEXTURE_2D deprecated in ES 2 
     * Use glActiveTexture() */
    // glEnable(GL_TEXTURE_2D);
    // checkGlError("glEnable(GL_TEXTURE_2D)");

    /*
    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    checkGlError("glActiveTexture");

    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, tex);
    checkGlError("glBindTexture");

    // Use tightly packed data
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    checkGlError("glPixelStorei()");

    // Generate a texture object
    glGenTextures(1, &tex);

    // // Load assets
    // int tex_w, tex_h; // Value init by libpng
    // GLubyte *img_pixels = getBytesFromPNG("assets/player.png", APKArchive, tex_w, tex_h);
    // if (img_pixels == NULL)
    //     LOGI("Failed to load tex");
    // LOGI("TEX Width:%i, TEX Height:%i", tex_w, tex_h);
    // LOGI("BYTEZ->%s", (char*)&img_pixels);
    // tex = createSimpleTexture2D(tex, img_pixels, tex_w, tex_h, 4);

    // delete[] &img_pixels;
    */

    return true;
}

// void Rend_player::renderFrame() {
//     float bg = 0.0f;
//     glClearColor(bg, bg, bg, 1.0f);
//     checkGlError("glClearColor");
//     glClear(GL_COLOR_BUFFER_BIT);
//     checkGlError("glClear");
//
//     glUseProgram(gProgram);
//     checkGlError("glUseProgram");
//
//     //---
//     // glGenTextures(1, &tex);
//     // glActiveTexture(GL_TEXTURE0);
//     // glBindTexture(GL_TEXTURE_2D, tex);
//     // GLubyte *img_pixels = SOIL_load_image("assets/player.mp3", &w, &h, 0, SOIL_LOAD_RGBA);
//     // tex = createSimpleTexture2D(tex, img_pixels, w, h, 4)
//
//     // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
//     // SOIL_free_image_data(image);
//
//     // Bind the texture
//     // glActiveTexture(GL_TEXTURE0);
//     // glBindTexture(GL_TEXTURE_2D, tex);
//     //
//     // // Set the sampler texture unit to 0
//     glUniform1i(gsTexHandle, 0);
//     checkGlError("glUniform1i(sTexture)");
//
//
//     /* load an image file directly as a new OpenGL texture */
//     // GLuint tex_2d = SOIL_load_OGL_texture
//     //     (
//     //      "assets/player.mp3",
//     //      SOIL_LOAD_AUTO,
//     //      SOIL_CREATE_NEW_ID,
//     //      SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
//     //     );
//     //
//     // glGenTextures(1, &tex_2d);
//     //
//     // glActiveTexture(GL_TEXTURE0);
//     // glBindTexture(GL_TEXTURE_2D, tex_2d);
//     //
//     // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
//     // glGenerateMipmap(GL_TEXTURE_2D);
//     // glBindTexture(GL_TEXTURE_2D, 0);
//     //
//     // glUniform1i(gsTexHandle, 0);
//     //---
//
//     // Enable Transparancy
//     glEnable(GL_BLEND);
//     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
// }
//

void Rend_player::renderObject(Object *obj) {
    // Positioning
    std::vector<float> objVertices = useObjectVertices(obj);

    // Colours
    Colour colour = Colour(0.0f, 0.0f, 1.0f, 1.0f);
    std::vector<float> objColours = useColour(&colour);

    // Rotate
    float objAngle = obj->getRotAngle();

    // Pass values to shader
    setShaderData(&objVertices[0], &objColours[0], objAngle);
}

std::vector<float> Rend_player::useObjectVertices(Object *obj) {
    /*  [p1]----[p3]   
         |        |  
         |        |
         |        |
         |(player)|
         |        |
         |        |
         |        |
        [p2]----[p4]  */


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
    float objAngle = obj->getRotAngle();

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

    /* Set base value (bottom of the player after rotation)
     * These calculations ensure that the base pos is the same as it would be after it went through the vshader.
     * The vshader rotates the pos, so the same rotation is applyed here */
    Point2D afterRot((vec[2] + vec[6])/2, (vec[3] + vec[7])/2);
    _base.setX(afterRot.getX()*cos(rad_angle) - afterRot.getY()*sin(rad_angle));
    _base.setY(afterRot.getY()*cos(rad_angle) + afterRot.getX()*sin(rad_angle));


    return std::vector<float> (vec, vec + sizeof(vec) / sizeof(float));
}

std::vector<float> Rend_player::useColour(Colour *colour) {
    float clr[] = { 0.8f     , 0.2f     , 0.3f     , colour->a,
                    colour->r, colour->g, colour->b, colour->a,
                    0.8f     , 0.2f     , 0.3f     , colour->a,
                    colour->r, colour->g, colour->b, colour->a};

    return std::vector<float> (clr, clr + sizeof(clr) / sizeof(float));
}

void Rend_player::setShaderData(float vertices[], float colours[], float angle) {
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
    checkGlError("player glDrawArrays");
}

void Rend_player::disableAttributes() {
    glDisableVertexAttribArray(gvPosHandle);
    glDisableVertexAttribArray(gvColorHandle);
}

Point2D Rend_player::getBasePoint() {
    return _base;
}

//----------------
zip_file* file = NULL;
void Rend_player::png_zip_read(png_structp png_ptr, png_bytep data, png_size_t length) {
    zip_fread(file, data, length);
}
GLubyte* Rend_player::getBytesFromPNG(const char* filename, zip *APKArchive, int &width, int &height) {
    file = zip_fopen(APKArchive, filename, 0);
    if (!file) {
        LOGE("Error opening %s from APK", filename);
        return TEXTURE_LOAD_ERROR;
    }

    //header for testing if it is a png
    png_byte header[8];

    //read the header
    zip_fread(file, header, 8);

    //test if png
    int is_png = !png_sig_cmp(header, 0, 8);
    if (!is_png) {
        zip_fclose(file);
        LOGE("Not a png file : %s", filename);
        return TEXTURE_LOAD_ERROR;
    }

    //create png struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
            NULL, NULL);
    if (!png_ptr) {
        zip_fclose(file);
        LOGE("Unable to create png struct : %s", filename);
        return (TEXTURE_LOAD_ERROR);
    }

    //create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
        LOGE("Unable to create png info : %s", filename);
        zip_fclose(file);
        return (TEXTURE_LOAD_ERROR);
    }

    //create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        LOGE("Unable to create png end info : %s", filename);
        zip_fclose(file);
        return (TEXTURE_LOAD_ERROR);
    }

    //png error stuff, not sure libpng man suggests this.
    if (setjmp(png_jmpbuf(png_ptr))) {
        zip_fclose(file);
        LOGE("Error during setjmp : %s", filename);
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        return (TEXTURE_LOAD_ERROR);
    }

    //init png reading
    //png_init_io(png_ptr, fp);
    png_set_read_fn(png_ptr, NULL, Rend_player::png_zip_read);

    //let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);

    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);

    //variables to pass to get info
    int bit_depth, color_type;
    png_uint_32 twidth, theight;

    // get info about png
    png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type,
            NULL, NULL, NULL);

    //update width and height based on png info
    width = twidth;
    height = theight;

    // Update the png info struct.
    png_read_update_info(png_ptr, info_ptr);

    // Row size in bytes.
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    // Allocate the image_data as a big block, to be given to opengl
    png_byte *image_data = new png_byte[rowbytes * height];
    if (!image_data) {
        //clean up memory and close stuff
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        LOGE("Unable to allocate image_data while loading %s ", filename);
        zip_fclose(file);
        return TEXTURE_LOAD_ERROR;
    }

    //row_pointers is for pointing to image_data for reading the png with libpng
    png_bytep *row_pointers = new png_bytep[height];
    if (!row_pointers) {
        //clean up memory and close stuff
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        delete[] image_data;
        LOGE("Unable to allocate row_pointer while loading %s ", filename);
        zip_fclose(file);
        return TEXTURE_LOAD_ERROR;
    }
    // set the individual row_pointers to point at the correct offsets of image_data
    for (int i = 0; i < height; ++i)
        row_pointers[height - 1 - i] = image_data + i * rowbytes;

    //read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);

    //Now generate the OpenGL texture object
    // GLuint texture;
    // glGenTextures(1, &texture);
    // glBindTexture(GL_TEXTURE_2D, texture);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
    //         GL_UNSIGNED_BYTE, (GLvoid*) image_data);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    //clean up memory and close stuff
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    // delete[] image_data;
    delete[] row_pointers;
    zip_fclose(file);

    // return texture;
    return image_data;
}

GLuint Rend_player::createSimpleTexture2D(GLuint _textureid, GLubyte* pixels, int width, int height, int channels) {

    GLenum format;
    switch (channels) {
        case 3:
            format = GL_RGB;
            break;
        case 1:
            format = GL_LUMINANCE;
            break;
        case 4:
            format = GL_RGBA;
            break;
    }
    // Load the texture
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, (GLvoid*) pixels);
    checkGlError("glTexImage2D");

    // Set the filtering mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

    return _textureid;
}
