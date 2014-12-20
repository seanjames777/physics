/**
 * @file demo.h
 *
 * @brief Demo application framework
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __DEMO_H
#define __DEMO_H

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <string>

class GLFWwindow;

namespace Physics {

class System;
class Body;

namespace Util {

class Mesh;
class RenderTarget;
class Shader;
class Camera;

/**
 * @brief Demo application base class
 */
class Demo {
private:

    struct MeshBodyPair {
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<Body> body;
    };

    int                                 width;                 //!< Window width
    int                                 height;                //!< Window height
    int                                 shadowSize;            //!< Shadow map width and height
    GLFWwindow                         *window;                //!< GLFW window
    bool                                close;                 //!< Whether demo should exit
    std::shared_ptr<Shader>             phong_shader;          //!< Default object shader
    std::shared_ptr<Shader>             shadow_shader;         //!< Shadow map shader
    std::shared_ptr<Shader>             flat_shader;           //!< Vertex color shader
    std::shared_ptr<Camera>             camera;                //!< Camera
    std::shared_ptr<System>             system;                //!< Physics system
    double                              time;                  //!< Current time
    std::vector<MeshBodyPair>           meshes;                //!< List of meshes to draw
    glm::vec3                           lightDir;              //!< Global light direction
    float                               shadowBounds;          //!< Shadow view rectangle width/height
    float                               shadowNear;            //!< Shadow near plane
    float                               shadowFar;             //!< Shadow far plane
    glm::mat4                           lightViewProjection;   //!< Shadow view * projectionm matrix
    std::shared_ptr<RenderTarget>       shadowTarget;          //!< Shadow render target
    std::shared_ptr<Mesh>               debug_mesh;            //!< Debug visualization mesh

    /**
     * @brief Handle window resize events
     *
     * @param[in] window Window that changed size
     * @param[in] width  New width
     * @param[in] height New height
     */
    static void resizeHandler(GLFWwindow *window, int width, int height);

    /**
     * @brief Handle mouse events
     *
     * @param[in] window Window that received event
     * @param[in] button Mouse button
     * @param[in] action Action: press or release
     * @param[in] mods   Modifier keys
     */
    static void mouseHandler(GLFWwindow *window, int button, int action, int mods);

    /**
     * @brief Get current time in seconds
     */
    static double getTime();

    /**
     * @brief Draw demo scene
     */
    void draw();

    /**
     * @brief Construct debug_mesh
     */
    void prepDebug();

protected:

    /**
     * @brief Overriden by derived classes to setup the demo scene
     */
    virtual void init_demo() = 0;

    /**
     * @brief Overriden by derived classes to tear down a demo
     */
    virtual void destroy_demo() = 0;

    /**
     * @brief Overriden by derived classes to handle mouse clicks
     */
    virtual void demo_mouseDown(int button) = 0; // TODO shouldnt all be =0

    /**
     * @brief Get the camera
     */
    std::shared_ptr<Camera> getCamera();

    /**
     * @brief Get physics system
     */
    std::shared_ptr<System> getSystem();

    /**
     * @brief Add a mesh to the list of meshes to draw. It will be drawn at
     * the location of the given body. Meshes may be added more than once
     * with different bodies. TODO params.
     */
    void addMesh(std::shared_ptr<Mesh> mesh, std::shared_ptr<Body> body);

public:

    /**
     * @brief Constructor
     */
    Demo(std::string title = "Physics Demo", int width = 1024,
        int height = 768, int shadowSize = 1024);

    /**
     * @brief Run demo to completion
     */
    void run();

    /**
     * @brief Destructor
     */
    virtual ~Demo();

};

}}

#endif
