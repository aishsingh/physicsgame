#include <stdlib.h>
#include <cmath>
#include "background_renderer.h"
#include "log.h"
#include "game.h"

BackgroundRenderer::BackgroundRenderer() {
    _shad_vertex =
        "attribute vec2 vPos;\n"

        "void main() {\n"
        "  gl_Position = vec4(vPos, 0.0, 1.0);\n"
        "}\n";

    _shad_fragment = 
        "precision mediump float;\n"

        "//behaves slightly differently on amd/nv\n"
        "uniform float iGlobalTime;\n"
        "uniform vec2 iResolution;\n"

        "vec3 nrand3( vec2 co )\n"
        "{\n"
            "vec3 a = fract( cos( co.x*8.3e-3 + co.y )*vec3(1.3e5, 4.7e5, 2.9e5) );\n"
            "vec3 b = fract( sin( co.x*0.3e-3 + co.y )*vec3(8.1e5, 1.0e5, 0.1e5) );\n"
            "vec3 c = mix(a, b, 0.5);\n"
            "return c;\n"
        "}\n"

        "void main() {\n"
            "vec2 offset = vec2(10., 10.);\n"
            "vec2 uv = 2. * gl_FragCoord.xy / iResolution.xy -1.;\n"
            "vec2 uvs = uv * iResolution.xy / max(iResolution.x, iResolution.y);\n"
            "vec2 p = (uvs/4.) + (0.15 * vec2(iGlobalTime / 16., iGlobalTime / 0.));\n"
            "p += 0.2 * vec2(sin(iGlobalTime / 16.), sin(iGlobalTime / 12.));\n"

            "//Thanks to http://glsl.heroku.com/e#6904.0\n"
            "vec2 seed = p.xy * 2.0;	\n"
            "seed = floor(seed * iResolution.x);\n"
            "vec3 rnd = nrand3( seed );\n"
            "vec4 starcolor = vec4(pow(rnd.y,40.0));\n"

            "for (float i=0.0; i<0.0; i++) {\n"
                "//Other Layers\n"
                "vec2 p2 = vec2(uvs / (4.+sin(iGlobalTime*0.11)*0.2+0.2+sin(iGlobalTime*0.15)*0.3+0.4)) + vec2(2., -1.3);\n"
                "p2 += (0.25 * i) * vec2(sin(iGlobalTime / 16.), sin(iGlobalTime / 12.));\n"

                "vec2 seed2 = p2.xy * 2.0;\n"
                "seed2 = floor(seed2 * iResolution.x);\n"
                "vec3 rnd2 = nrand3( seed2 );\n"
                "starcolor += vec4(pow(rnd2.y,40.0));\n"
            "}\n"
            "gl_FragColor = starcolor;\n"
        "}\n";

    _program = createProgram(_shad_vertex.c_str(), _shad_fragment.c_str());
    if (!_program) {
        LOGE("Could not create program.");
    }

    _vPos_handle = glGetAttribLocation(_program, "vPos");
    checkGlError("glGetAttribLocation(vPos)");

    _time_handle = glGetUniformLocation(_program, "iGlobalTime");
    checkGlError("glGetUniformLocation(time)");

    _res_handle = glGetUniformLocation(_program, "iResolution");
    checkGlError("glGetUniformLocation(res)");
}

void BackgroundRenderer::render() {
    // Change renderer
    glUseProgram(_program);

    glUniform1f(_time_handle, 0);
    // LOGI("time %.2f", Game::getElapsedTime()/2);
    checkGlError("glUniform1f");

    float res[] {
        static_cast<float>(Game::getScreenWidth()),
        static_cast<float>(Game::getScreenHeight())
    };
    glUniform2fv(_res_handle, 1, res);
    checkGlError("glUniform2fv");

    float vertices[] {
        -1,1, 
         1,1, 
        -1,-1, 
         1,-1
    };

    glVertexAttribPointer(_vPos_handle, 2, GL_FLOAT, GL_FALSE, 0, &vertices[0]);
    checkGlError("glVertexAttribPointer");

    glEnableVertexAttribArray(_vPos_handle);
    checkGlError("glEnableVertexAttribArray");

    // Pass attributes to shader
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    checkGlError("glDrawArrays");

    disableAttributes();
}

void BackgroundRenderer::disableAttributes() {
    // Any unique attributes in these shaders needs to be disabled here
    glDisableVertexAttribArray(_vPos_handle);
}
