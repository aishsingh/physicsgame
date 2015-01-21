#include <stdlib.h>
#include "asset_renderer.h"
#include "log.h"
#include "game.h"

#define PI 3.14159265358979323846264
#define TEXTURE_LOAD_ERROR 0

AssetRenderer::AssetRenderer(Camera *cam) : _cam(cam) {
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

    _gProgram = createProgram(_shad_vertex.c_str(), _shad_fragment.c_str());
    if (!_gProgram) {
        LOGE("Could not create program.");
    }
    _gvPosHandle = glGetAttribLocation(_gProgram, "vPos");
    checkGlError("glGetAttribLocation(vPos)");

    _gvColorHandle = glGetAttribLocation(_gProgram, "vColor");
    checkGlError("glGetAttribLocation(vColor)");

    _gsTexHandle = glGetUniformLocation(_gProgram, "sTexture");
    checkGlError("glGetUniformLocation(sTexture)");

    _gmMVPHandle = glGetUniformLocation(_gProgram, "mMVP");
    checkGlError("glGetUniformLocation(mMVP)");
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
}

// void AssetRenderer::renderFrame() {
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

void AssetRenderer::render(vector<float> vertices, vector<float> colours, float angle, GLenum mode) {
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
    Point2D anchor_pt = _cam->getPos();
    glm::mat4 view_mat;
    view_mat = glm::translate(view_mat, glm::vec3(ctr.getX() - anchor_pt.getX(), ctr.getY() - anchor_pt.getY(), 0));
    view_mat = glm::translate(view_mat, glm::vec3(anchor_pt.getX(), anchor_pt.getY(), 0));
    view_mat = glm::rotate(view_mat, 
                           static_cast<float>(_cam->getRotAngle()*PI/180), 
                           glm::vec3(0.0f, 0.0f, 1.0f));
    view_mat = glm::scale(view_mat, glm::vec3(_cam->getScale(), _cam->getScale(), _cam->getScale()));
    view_mat = glm::translate(view_mat, glm::vec3(-anchor_pt.getX(), -anchor_pt.getY(), 0));

    // MVP
    glm::mat4 MVP_mat = _proj_mat * view_mat * model_mat;

    // Pass MVP to shader
    glUniformMatrix4fv(_gmMVPHandle, 1, GL_FALSE, glm::value_ptr(MVP_mat));
    checkGlError("glUniformMatrix4fv, mMVP");

    glVertexAttribPointer(_gvPosHandle, 2, GL_FLOAT, GL_FALSE, 0, &vertices[0]);
    glVertexAttribPointer(_gvColorHandle, 4, GL_FLOAT, GL_FALSE, 0, &colours[0]);
    checkGlError("glVertexAttrib");

    glEnableVertexAttribArray(_gvPosHandle);
    glEnableVertexAttribArray(_gvColorHandle);
    checkGlError("glEnableVertexAttribArray");

    // Pass attributes to shader
    glDrawArrays(mode, 0, vertices.size()/2);
    checkGlError("player glDrawArrays");
}

void AssetRenderer::disableAttributes() {
    glDisableVertexAttribArray(_gvPosHandle);
    glDisableVertexAttribArray(_gvColorHandle);
}

//----------------
zip_file* file = NULL;
void AssetRenderer::png_zip_read(png_structp png_ptr, png_bytep data, png_size_t length) {
    zip_fread(file, data, length);
}
GLubyte* AssetRenderer::getBytesFromPNG(const char* filename, zip *APKArchive, int &width, int &height) {
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
    png_set_read_fn(png_ptr, NULL, AssetRenderer::png_zip_read);

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

GLuint AssetRenderer::createSimpleTexture2D(GLuint _textureid, GLubyte* pixels, int width, int height, int channels) {

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
