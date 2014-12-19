/**
 * @file demo.h
 *
 * @brief Demo application framework
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __DEMO_H
#define __DEMO_H

#include <physics/system.h>
#include <util/shader.h>
#include <util/camera.h>
#include <memory>

class GLFWwindow;

namespace Physics { namespace Util {

/**
 * @brief Demo application base class
 */
class Demo {
private:

    int                      width;  //!< Window width
    int                      height; //!< Window height
    GLFWwindow              *window; //!< GLFW window
    bool                     close;  //!< Whether demo should exit
    std::shared_ptr<Shader>  shader; //!< Default object shader
    std::shared_ptr<Camera>  camera; //!< Camera
    std::shared_ptr<System>  system; //!< Physics system
    double                   time;   //!< Current time

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

protected:

    /**
     * @brief Overriden by derived classes to setup the demo scene
     */
    virtual void init_demo() = 0;

    /**
     * @brief Overriden by derived classes to draw a frame
     */
    virtual void draw_demo() = 0;

    /**
     * @brief Overriden by derived classes to tear down a demo
     */
    virtual void destroy_demo() = 0;

    /**
     * @brief Overriden by derived classes to handle mouse clicks
     */
    virtual void demo_mouseDown(int button) = 0; // TODO shouldnt all be =0

    /**
     * @brief Get the default object shader
     */
    std::shared_ptr<Shader> getDefaultShader();

    /**
     * @brief Get the camera
     */
    std::shared_ptr<Camera> getCamera();

    /**
     * @brief Get physics system
     */
    std::shared_ptr<System> getSystem();

public:

    /**
     * @brief Constructor
     */
    Demo();

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
