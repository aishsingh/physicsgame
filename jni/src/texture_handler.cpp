#include "texture_handler.h"
#include "config.h"
#include "log.h"

#define TEXTURE_LOAD_ERROR 0

zip_file* TextureHandler::_tmp_file(0);

TextureHandler::TextureHandler() { }
TextureHandler::~TextureHandler() {
    zip_close(_APK);
    _APK = NULL;
    _tmp_file = NULL;
}

void TextureHandler::loadAPK(const char *package_name) {
    LOGI("-> Opening APK");
    _APK = zip_open(package_name, 0, NULL);
    if (_APK == NULL) {
        LOGE("Error loading APK");
        return;
    }

    if (OUT_TEX_APK_CONTENTS) {
        int numFiles = zip_get_num_files(_APK);
        for (int i=0; i<numFiles; i++) {
            const char* name = zip_get_name(_APK, i, 0);
            if (name == NULL) {
                LOGE("Error reading zip file name at index %i : %s", i, zip_strerror(_APK));
                return;
            }
            LOGI("-> File %i : %s\n", i, name);
        }
    }
}

void TextureHandler::loadTextures() {
    GLuint tex;
    tex = loadTexFromPNG("assets/spritesheets/spaceman_run.png");
    if (tex != TEXTURE_LOAD_ERROR)
        _textures.push_back(tex);

    // ...
    // tex = loadTexFromPNG("assets/...png");
    // if (tex != TEXTURE_LOAD_ERROR)
    //     _textures.push_back(tex);
}

GLuint TextureHandler::getTex(TextureName name) {
    return _textures.at(name);
}

GLuint TextureHandler::loadTexFromPNG(const char* filename) {
    _tmp_file = zip_fopen(_APK, filename, 0);
    if (!_tmp_file) {
        LOGE("Error opening %s from APK", filename);
        return TEXTURE_LOAD_ERROR;
    }

    //header for testing if it is a png
    png_byte header[8];

    //read the header
    zip_fread(_tmp_file, header, 8);

    //test if png
    int is_png = !png_sig_cmp(header, 0, 8);
    if (!is_png) {
        zip_fclose(_tmp_file);
        LOGE("Not a png file : %s", filename);
        return TEXTURE_LOAD_ERROR;
    }

    //create png struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
            NULL, NULL);
    if (!png_ptr) {
        zip_fclose(_tmp_file);
        LOGE("Unable to create png struct : %s", filename);
        return (TEXTURE_LOAD_ERROR);
    }

    //create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
        LOGE("Unable to create png info : %s", filename);
        zip_fclose(_tmp_file);
        return (TEXTURE_LOAD_ERROR);
    }

    //create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        LOGE("Unable to create png end info : %s", filename);
        zip_fclose(_tmp_file);
        return (TEXTURE_LOAD_ERROR);
    }

    //png error stuff, not sure libpng man suggests this.
    if (setjmp(png_jmpbuf(png_ptr))) {
        zip_fclose(_tmp_file);
        LOGE("Error during setjmp : %s", filename);
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        return (TEXTURE_LOAD_ERROR);
    }

    //init png reading
    //png_init_io(png_ptr, fp);
    png_set_read_fn(png_ptr, NULL, TextureHandler::png_zip_read);

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

    // Update the png info struct.
    png_read_update_info(png_ptr, info_ptr);

    // Row size in bytes.
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    // Allocate the image_data as a big block, to be given to opengl
    png_byte *image_data = new png_byte[rowbytes * theight];
    if (!image_data) {
        //clean up memory and close stuff
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        LOGE("Unable to allocate image_data while loading %s ", filename);
        zip_fclose(_tmp_file);
        return TEXTURE_LOAD_ERROR;
    }

    //row_pointers is for pointing to image_data for reading the png with libpng
    png_bytep *row_pointers = new png_bytep[theight];
    if (!row_pointers) {
        //clean up memory and close stuff
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        delete[] image_data;
        LOGE("Unable to allocate row_pointer while loading %s ", filename);
        zip_fclose(_tmp_file);
        return TEXTURE_LOAD_ERROR;
    }
    // set the individual row_pointers to point at the correct offsets of image_data
    for (int i = 0; i < (int)theight; ++i)
        row_pointers[theight - 1 - i] = image_data + i * rowbytes;

    //read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);

    //Now generate the OpenGL texture object
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, twidth, theight, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, (GLvoid*) image_data);

    // Apply options/filters
    glTexParameteri(GL_TEXTURE_2D, 
                    GL_TEXTURE_MIN_FILTER, 
                    GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, 
                    GL_TEXTURE_MAG_FILTER, 
                    GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S,
                    GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T,
                    GL_CLAMP_TO_EDGE);

    LOGI("-> Loaded '%s' : [%d x %d]", filename, twidth, theight);

    //clean up memory and close stuff
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    delete[] image_data;
    delete[] row_pointers;
    zip_fclose(_tmp_file);

    return texture;
}

void TextureHandler::png_zip_read(png_structp png_ptr, png_bytep data, png_size_t length) {
    zip_fread(_tmp_file, data, length);
}

std::vector<float> TextureHandler::calcTexVerticesFromSpritesheet(float spritesheet_w, float spritesheet_h, int sprite_hori_count, int sprite_vert_count, int index, bool flip) {
    std::vector<float> vert;
    float sprite_w = spritesheet_w/sprite_hori_count;
    float sprite_h = spritesheet_h/sprite_vert_count;

    /* Texture coordinate origin is at the bottom left of the spritesheet
         ^
         |
       --|---->
         |
     */

    int cell_x = 0;
    int cell_y = sprite_vert_count-1;

    if (index < sprite_hori_count)
        cell_x = index;
    else {
        cell_y = sprite_hori_count - (index/sprite_hori_count);
        cell_x = index - ((sprite_hori_count-cell_y)*sprite_hori_count);
    }

    if (OUT_TEX_ANIMATION_CELL)
        LOGI("Sprite cell (%i, %i), index %i", cell_x, cell_y, index);

    // convert to actual coordinates
    cell_x *= sprite_w;
    cell_y *= sprite_h;

    // pts used to know the boundaries of the sprite (pts are on opposite corners)
    Point2D pt1 = Point2D(cell_x/spritesheet_w, cell_y/spritesheet_h);
    Point2D pt2 = Point2D((cell_x+sprite_w)/spritesheet_w, (cell_y+sprite_h)/spritesheet_h);

    // flip texture horizontally
    if (flip) {
        float tex_left_vert[] = {
            pt2.getX(), pt2.getY(),
            pt2.getX(), pt1.getY(),
            pt1.getX(), pt2.getY(),
            pt1.getX(), pt1.getY()
        };

        vert = std::vector<float> (tex_left_vert, tex_left_vert + sizeof(tex_left_vert) / sizeof(float));
    }
    else {
        float tex_default_vert[] = {
            pt1.getX(), pt2.getY(),
            pt1.getX(), pt1.getY(),
            pt2.getX(), pt2.getY(),
            pt2.getX(), pt1.getY()
        };

        vert = std::vector<float> (tex_default_vert, tex_default_vert + sizeof(tex_default_vert) / sizeof(float));
    }
    
    return vert;
}
