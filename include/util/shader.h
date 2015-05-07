#ifndef __SHADER_H
#define __SHADER_H

#include <util/gldefs.h>
#include <string>

enum ShaderStage {
    ShaderStageVS  = GL_VERTEX_SHADER,
    ShaderStageTCS = GL_TESS_CONTROL_SHADER,
    ShaderStageTES = GL_TESS_EVALUATION_SHADER,
    ShaderStageGS  = GL_GEOMETRY_SHADER,
    ShaderStageFS  = GL_FRAGMENT_SHADER
};

class Shader {
private:

    GLuint program;
    bool   _error;
    GLuint vs, tes, tcs, gs, fs;

public:

    Shader();

    ~Shader();

    void setStage(ShaderStage stage, const std::string & file);

    bool error();

    void finish();

    void bind();

    void unbind();

    GLuint getGLProgram();

    void setUniformBufferBinding(const char *bufferName, unsigned int binding);

    void setTextureBinding(const char *textureName, unsigned int binding);

};

#endif
