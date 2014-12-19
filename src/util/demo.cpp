/**
 * @file demo.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <util/demo.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <iostream>
#include <cassert>
#include <sys/time.h>

using namespace std::placeholders;

namespace Physics { namespace Util {

double Demo::getTime() {
    struct timeval tv;
    gettimeofday(&tv, nullptr);

    double time =
        (double)tv.tv_sec +
        (double)tv.tv_usec * (1.0 / 1000000.0);

    return time;
}

void Demo::resizeHandler(GLFWwindow *window, int width, int height) {
    Demo *demo = (Demo *)glfwGetWindowUserPointer(window);

    demo->width = width;
    demo->height = height;

    glViewport(0, 0, width, height);

    demo->camera->setAspectRatio((float)width / (float)height);
}

void Demo::mouseHandler(GLFWwindow *window, int button, int action, int mods) {
    if (action != GLFW_PRESS)
        return;

    Demo *demo = (Demo *)glfwGetWindowUserPointer(window);

    demo->demo_mouseDown(button);
}

std::shared_ptr<Shader> Demo::getDefaultShader() {
    return shader;
}

std::shared_ptr<Camera> Demo::getCamera() {
    return camera;
}

std::shared_ptr<System> Demo::getSystem() {
    return system;
}

Demo::Demo()
    : width(1024),
      height(768),
      window(nullptr),
      close(false),
      shader(nullptr),
      camera(nullptr),
      system(nullptr),
      time(0.0)
{
    glfwInit();

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);

    window = glfwCreateWindow(width, height, "Physics", NULL, NULL);

    glfwSetWindowUserPointer(window, this);
    glfwSetWindowSizeCallback(window, resizeHandler);
    glfwSetMouseButtonCallback(window, mouseHandler);

    glfwMakeContextCurrent(window);

    glewExperimental = true;
    glewInit();
    glGetError(); // Eat spurious error

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE); // TODO
    glEnable(GL_MULTISAMPLE);

    shader = std::make_shared<Shader>();
    bool stat = false;
    stat = shader->setVS("content/shaders/phong.vs");
    assert(stat);
    stat = shader->setFS("content/shaders/phong.fs");
    assert(stat);
    stat = shader->link();
    assert(stat);

    camera = std::make_shared<Camera>(glm::vec3(), glm::vec3());
    camera->setAspectRatio((float)width / (float)height);

    system = std::make_shared<System>();
}

void Demo::run() {
    init_demo();

    time = getTime();

    while (!glfwWindowShouldClose(window) && !close) {
        glfwPollEvents();

        glClearColor(0.2f, 0.4f, 0.8f, 1.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double newTime = getTime();
        double elapsed = newTime - time;
        time = newTime;
        system->integrate(newTime, elapsed);

        draw_demo();

        glfwSwapBuffers(window);
    }

    destroy_demo();
}

Demo::~Demo() {
    glfwDestroyWindow(window);

    glfwTerminate();
}

}}
