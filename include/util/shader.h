/**
 * @file shader.h
 *
 * @brief Shader compilation utility
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __SHADER_H
#define __SHADER_H

#include <string>
#include <GL/glew.h>

namespace Physics { namespace Util {

/**
 * @brief Shader wrapper class
 */
class Shader {
private:

    GLuint vs;      //!< Vertex shader
    GLuint gs;      //!< Geometry shader
    GLuint tcs;     //!< Tessellation control shader
    GLuint tes;     //!< Tessellation evaluation shader
    GLuint fs;      //!< Fragment shader
    GLuint program; //!< Program

    /**
     * @brief Shader compilation utility
     *
     * @param[in] stage    Shader type
     * @param[in] filename Shader filename
     *
     * @return Compiled shader or 0
     */
    static GLint compile_shader(GLenum stage, std::string filename);

    /**
     * @brief Program link utility
     *
     * @param[in] program Program to link
     *
     * @return False on error, or true
     */
    static bool link_program(GLuint program);

public:

    /**
     * @brief Constructor
     */
    Shader();

    /**
     * @brief Destructor
     */
    ~Shader();

    /**
     * @brief Compile and set the vertex shader
     */
    bool setVS(std::string file);

    /**
     * @brief Compile and set the geometry shader
     */
    bool setGS(std::string file);

    /**
     * @brief Compile and set the tessellation control shader
     */
    bool setTCS(std::string file);

    /**
     * @brief Compile and set the tessellation evaluation shader
     */
    bool setTES(std::string file);

    /**
     * @brief Compile and set the fragment shader
     */
    bool setFS(std::string file);

    /**
     * @brief Link the program
     */
    bool link();

    /**
     * @brief Get the OpenGL program object
     */
    GLuint getProgram();

    /**
     * @brief Bind shader for rendering
     */
    void bind();

    /**
     * @brief Un-bind shader
     */
    void unbind();

};

}}

#endif
