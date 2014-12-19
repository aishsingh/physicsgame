#ifndef REND_PLAYER_H
#define REND_PLAYER_H

#include <GLES2/gl2.h>
#include "renderer.h"

class Player;

class Rend_player: public Renderer {
    private:
        GLuint gsTexHandle;

        // PNG loading
        static void png_zip_read(png_structp png_ptr, png_bytep data, png_size_t length);
        GLubyte* getBytesFromPNG(const char* filename, zip *APKArchive, int &width, int &height);

        // Texture loading
        GLuint tex;
        GLuint createSimpleTexture2D(GLuint _textureid, GLubyte* pixels, int width, int height, int channels);

        std::vector<float> useObjectVertices(Object *obj);
        std::vector<float> useColour(Colour *colour); 
        void setShaderData(float vertices[], float colours[], float angle);

    public:
        void renderPlayer(Player *pla);
        void disableAttributes();
        bool setup();

        /* Ctor - Init Shaders */
        Rend_player();
};

#endif /* REND_PLAYER_H */
