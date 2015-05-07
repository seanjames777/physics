#include <util/shader.h>

#include <iostream>
#include <fstream>

Shader::Shader()
    : _error(false),
      vs(0),
      tes(0),
      tcs(0),
      gs(0),
      fs(0)
{
    program = glCreateProgram();
}

Shader::~Shader() {
    glDeleteProgram(program);
}

void Shader::setStage(ShaderStage stage, const std::string & path) {
    GLuint *shader = nullptr;

    switch(stage) {
    case ShaderStageVS:
        shader = &vs;
        break;
    case ShaderStageTES:
        shader = &tes;
        break;
    case ShaderStageTCS:
        shader = &tcs;
        break;
    case ShaderStageGS:
        shader = &gs;
        break;
    case ShaderStageFS:
        shader = &fs;
        break;
    }

    *shader = glCreateShader((GLenum)stage);

    std::ifstream file(path, std::ios::in | std::ios::binary);

    if (!file) {
        std::cout << "Error opening '" << path << "'" << std::endl;
        glDeleteShader(*shader);
        *shader = 0;
        _error = true;
        return;
    }

    file.seekg(0, std::ios::end);
    int size = (int)file.tellg(); // int because glShaderSource wants an int
    file.seekg(0, std::ios::beg);

    char *src = new char[size];
    file.read(src, size);

	if (!file) {
		std::cout << "Error reading from '" << path << "' (" << file.gcount() << ") characters read" << std::endl;
		delete[] src;
		file.close();
		glDeleteShader(*shader);
		*shader = 0;
		_error = true;
		return;
	}

    file.close();

	glShaderSource(*shader, 1, (const char **)&src, &size);
    delete [] src;

    glCompileShader(*shader);

    int status;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);

    if (!status) {
        std::cout << "Error compiling shader '" << path << "':" << std::endl;

        int logsize = 0;
        glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logsize);

        char *log = new char[logsize];

        glGetShaderInfoLog(*shader, logsize, &logsize, log);

        std::cout << log << std::endl;

        delete [] log;
        glDeleteShader(*shader);
        *shader = 0;
        _error = true;
        return;
    }

    glAttachShader(program, *shader);
}

void Shader::finish() {
    glLinkProgram(program);

    int status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);

    if (!status) {
        std::cout << "Error linking shader program:" << std::endl;

        int logsize = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logsize);

        char *log = new char[logsize];

        glGetProgramInfoLog(program, logsize, &logsize, log);

        std::cout << log << std::endl;

        delete [] log;

        _error = true;
    }

    if (vs) {
        glDetachShader(program, vs);
        glDeleteShader(vs);
    }

    if (tes) {
        glDetachShader(program, tes);
        glDeleteShader(tes);
    }

    if (tes) {
        glDetachShader(program, tes);
        glDeleteShader(tes);
    }

    if (tcs) {
        glDetachShader(program, tcs);
        glDeleteShader(tcs);
    }

    if (fs) {
        glDetachShader(program, fs);
        glDeleteShader(fs);
    }
}

bool Shader::error() {
    return _error;
}

void Shader::bind() {
    glUseProgram(program);
}

void Shader::unbind() {
    glUseProgram(0);
}

GLuint Shader::getGLProgram() {
    return program;
}

void Shader::setUniformBufferBinding(const char *bufferName, unsigned int binding) {
    GLuint location = glGetUniformBlockIndex(program, bufferName);

    if (location != GL_INVALID_INDEX)
        glUniformBlockBinding(program, location, binding);
}

void Shader::setTextureBinding(const char *textureName, unsigned int binding) {
    GLint location = glGetUniformLocation(program, textureName);

    if (location >= 0)
        glProgramUniform1i(program, location, binding);
}
