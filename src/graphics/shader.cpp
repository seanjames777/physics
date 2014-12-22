/**
 * @file shader.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <graphics/shader.h>
#include <graphics/path.h>
#include <fstream>
#include <iostream>
#include <cassert>

namespace Graphics {

GLint Shader::compile_shader(GLenum stage, std::string filename) {
    std::string path = PathUtil::prependExecutableDirectory(filename);

    std::ifstream in_src(path, std::ifstream::binary);

    if (!in_src) {
        std::cout << "Could not open " << path << std::endl;
        return 0;
    }

    int length = 0;
    in_src.seekg(0, in_src.end);
    length = in_src.tellg();
    in_src.seekg(0, in_src.beg);

    char *buffer = new char[length]; // TODO: could fail
    in_src.read(buffer, length);

    if (!in_src) {
        delete [] buffer;
        in_src.close();
        return 0;
    }

    in_src.close();

    GLuint shader = glCreateShader(stage);

    if (!shader)
        return 0;

    glShaderSource(shader, 1, (const char **)&buffer, &length);
    delete [] buffer;

    glCompileShader(shader);

    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (!status) {
        std::cout << "Shader compilation failed: " << filename << std::endl;

        int buffsz = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &buffsz);

        char *buff = new char[buffsz]; // TODO: could fail
        glGetShaderInfoLog(shader, buffsz, &buffsz, buff);
        std::cout << buff << std::endl;
        delete [] buff;

        return 0;
    }

    return shader;
}

bool Shader::link_program(GLuint program) {
    glLinkProgram(program);

    int status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);

    if (!status) {
        std::cout << "Program link failed" << std::endl;

        int buffsz = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &buffsz);

        char *buff = new char[buffsz]; // TODO: could fail
        glGetProgramInfoLog(program, buffsz, &buffsz, buff);
        std::cout << buff << std::endl;
        delete [] buff;

        return false;
    }

    return true;
}

Shader::Shader()
    : vs(0),
      gs(0),
      tcs(0),
      tes(0),
      fs(0),
      program(0)
{
    program = glCreateProgram(); // TODO: might fail
}

Shader::~Shader() {
    if (program)
        glDeleteProgram(program);
}

bool Shader::setVS(std::string file) {
    if ((vs = compile_shader(GL_VERTEX_SHADER, file)) == 0)
        return false;

    glAttachShader(program, vs);

    return true;
}

bool Shader::setGS(std::string file) {
    if ((gs = compile_shader(GL_GEOMETRY_SHADER, file)) == 0)
        return false;

    glAttachShader(program, gs);

    return true;
}

bool Shader::setTCS(std::string file) {
    if ((tcs = compile_shader(GL_TESS_CONTROL_SHADER, file)) == 0)
        return false;

    glAttachShader(program, tcs);

    return true;
}

bool Shader::setTES(std::string file) {
    if ((tes = compile_shader(GL_TESS_EVALUATION_SHADER, file)) == 0)
        return false;

    glAttachShader(program, tes);

    return true;
}

bool Shader::setFS(std::string file) {
    if ((fs = compile_shader(GL_FRAGMENT_SHADER, file)) == 0)
        return false;

    glAttachShader(program, fs);

    return true;
}

bool Shader::link() {
    if (!link_program(program))
        return false;

    if (vs)
        glDeleteShader(vs);

    if (gs)
        glDeleteShader(gs);

    if (tcs)
        glDeleteShader(tcs);

    if (tes)
        glDeleteShader(tes);

    if (fs)
        glDeleteShader(fs);

    return true;
}

GLuint Shader::getProgram() {
    return program;
}

void Shader::bind() {
    glUseProgram(program);
}

void Shader::unbind() {
    glUseProgram(0);
}

}
