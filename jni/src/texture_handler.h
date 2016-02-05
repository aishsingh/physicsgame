/* Description: Loads all textures here
 * Created On: 2015-01-23 
 */

#ifndef TEXTURE_HANDLER_H
#define TEXTURE_HANDLER_H

#include <GLES2/gl2.h>
#include <zip.h>
#include <png.h>
#include <vector>
#include "rect.h"

typedef enum TextureName {
    TEX_SPACEMAN=0
} TextureName;

class TextureHandler {
    private:
        std::vector<GLuint> _textures;

        zip *_APK;
        static zip_file *_tmp_file;
        static void png_zip_read(png_structp png_ptr, png_bytep data, png_size_t length);
        GLuint loadTexFromPNG(const char* filename);

    public:
        /* Load assets in the apk by extracting it with libzip */
        void loadAPK(const char *package_name);
        void loadTextures();
        GLuint getTex(TextureName name);
        static std::vector<float> calcTexVerticesFromSpritesheet(float spritesheet_w, float spritesheet_h, int sprite_hori_count, int sprite_vert_count, int index, bool flip = false);

        /* Ctor / Dtor */
        TextureHandler();
        ~TextureHandler();
};

#endif /* TEXTURE_HANDLER_H */
