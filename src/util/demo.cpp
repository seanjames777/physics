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
#include <util/graphics/mesh.h>
#include <util/graphics/camera.h>
#include <util/graphics/shader.h>
#include <util/graphics/rendertarget.h>
#include <physics/system.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <physics/collision/shape.h>
#include <physics/dynamics/body.h>
#include <util/graphics/font.h>

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

std::shared_ptr<Camera> Demo::getCamera() {
    return camera;
}

std::shared_ptr<System> Demo::getSystem() {
    return system;
}

void Demo::addMesh(std::shared_ptr<Mesh> mesh, std::shared_ptr<Body> body) {
    MeshBodyPair pair;
    pair.mesh = mesh;
    pair.body = body;

    meshes.push_back(pair);
}

Demo::Demo(std::string title, int width, int height, int shadowSize)
    : width(width),
      height(height),
      shadowSize(shadowSize),
      window(nullptr),
      close(false),
      phong_shader(nullptr),
      shadow_shader(nullptr),
      font(nullptr),
      camera(nullptr),
      system(nullptr),
      time(0.0),
      debug_time(0.0),
      lightDir(glm::normalize(glm::vec3(0.8f, 1.0f, 0.6f))),
      shadowBounds(50.0f),
      shadowNear(-50.0f),
      shadowFar(50.0f),
      shadowTarget(nullptr),
      frameTime(0.0),
      physicsTime(0.0),
      nFrames(0)
{
    glfwInit();

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);

    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    glfwMakeContextCurrent(window);

    glfwSetWindowUserPointer(window, this);
    glfwSetWindowSizeCallback(window, resizeHandler);
    glfwSetMouseButtonCallback(window, mouseHandler);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glewExperimental = true;
    glewInit();
    glGetError(); // Eat spurious error

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE); // TODO
    //glEnable(GL_MULTISAMPLE);

    glfwSwapInterval(1);

    phong_shader = std::make_shared<Shader>();
    bool stat = false;
    stat = phong_shader->setVS("content/shaders/phong.vs");
    assert(stat);
    stat = phong_shader->setFS("content/shaders/phong.fs");
    assert(stat);
    stat = phong_shader->link();
    assert(stat);

    shadow_shader = std::make_shared<Shader>();
    stat = false;
    stat = shadow_shader->setVS("content/shaders/shadow.vs");
    assert(stat);
    stat = shadow_shader->setFS("content/shaders/shadow.fs");
    assert(stat);
    stat = shadow_shader->link();
    assert(stat);

    flat_shader = std::make_shared<Shader>();
    stat = false;
    stat = flat_shader->setVS("content/shaders/flat.vs");
    assert(stat);
    stat = flat_shader->setFS("content/shaders/flat.fs");
    assert(stat);
    stat = flat_shader->link();
    assert(stat);

    camera = std::make_shared<Camera>(glm::vec3(), glm::vec3());
    camera->setAspectRatio((float)width / (float)height);

    system = std::make_shared<System>();

    // TODO setLightDirection()
    glm::mat4 shadowView = glm::lookAt(lightDir, glm::vec3(), glm::vec3(0, 1, 0));
    glm::mat4 shadowProj = glm::ortho(
        -shadowBounds, shadowBounds, -shadowBounds, shadowBounds, shadowNear, shadowFar);
    lightViewProjection = shadowProj * shadowView;

    shadowTarget = std::make_shared<RenderTarget>(shadowSize, shadowSize, 0);
    shadowTarget->addColorTarget(GL_R32F, GL_LINEAR); // TODO format, filter
    shadowTarget->finish();

    debug_mesh = std::make_shared<Mesh>();

    font = std::make_shared<Font>("content/fonts/consolas_bold.ttf", 0, 12);
}

void Demo::draw() {
    glm::mat4 camViewProjection = camera->getViewProjection();

    shadowTarget->bind();

    glViewport(0, 0, shadowSize, shadowSize);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shadow_shader->bind();

    // TODO cache maybe
    GLint worldInverseTransposeLocation = glGetUniformLocation(
        shadow_shader->getProgram(), "worldInverseTranspose");
    GLint worldViewProjectionLocation = glGetUniformLocation(
        shadow_shader->getProgram(), "worldViewProjection"); // TODO unused
    GLint lightWorldViewProjectionLocation = glGetUniformLocation(
        shadow_shader->getProgram(), "lightWorldViewProjection");

    for (auto & pair : meshes) {
        // TODO redundant
        glm::mat4 world = pair.body->getTransform();
        glm::mat4 worldInverseTranspose = glm::inverse(glm::transpose(world));
        glm::mat4 worldViewProjection = camViewProjection * world;
        glm::mat4 lightWorldViewProjection = lightViewProjection * world;

        glUniformMatrix4fv(worldViewProjectionLocation, 1, false,
            glm::value_ptr(worldViewProjection));
        glUniformMatrix4fv(lightWorldViewProjectionLocation, 1, false,
            glm::value_ptr(lightWorldViewProjection));
        glUniformMatrix4fv(worldInverseTransposeLocation, 1, false,
            glm::value_ptr(worldInverseTranspose));

        pair.mesh->draw();
    }

    shadow_shader->unbind();

    shadowTarget->unbind();

    phong_shader->bind();

    worldInverseTransposeLocation = glGetUniformLocation(
        phong_shader->getProgram(), "worldInverseTranspose");
    worldViewProjectionLocation = glGetUniformLocation(
        phong_shader->getProgram(), "worldViewProjection");
    lightWorldViewProjectionLocation = glGetUniformLocation(
        phong_shader->getProgram(), "lightWorldViewProjection");
    GLint depthTextureLocation = glGetUniformLocation(
        phong_shader->getProgram(), "depthTexture");
    GLint lightDirectionLocation = glGetUniformLocation(
        phong_shader->getProgram(), "lightDirection");
    GLint depthTextureSizeLocation = glGetUniformLocation(
        phong_shader->getProgram(), "depthTextureSize");

    glViewport(0, 0, width, height);

    glClearColor(0.2f, 0.4f, 0.8f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    phong_shader->bind();

    shadowTarget->bindColorTexture(0, 0);

    for (auto & pair : meshes) {
        // TODO redundant
        glm::mat4 world = pair.body->getTransform();

        glm::mat4 worldInverseTranspose = glm::inverse(glm::transpose(world));
        glm::mat4 worldViewProjection = camViewProjection * world;
        glm::mat4 lightWorldViewProjection = lightViewProjection * world;

        glUniformMatrix4fv(worldViewProjectionLocation, 1, false,
            glm::value_ptr(worldViewProjection));
        glUniformMatrix4fv(lightWorldViewProjectionLocation, 1, false,
            glm::value_ptr(lightWorldViewProjection));
        glUniformMatrix4fv(worldInverseTransposeLocation, 1, false,
            glm::value_ptr(worldInverseTranspose));
        glUniform1i(depthTextureLocation, 0);
        glUniform1i(depthTextureSizeLocation, shadowSize);
        glUniform3fv(lightDirectionLocation, 1,
            glm::value_ptr(lightDir));

        pair.mesh->draw();
    }

    shadowTarget->unbindColorTexture(0);

    phong_shader->unbind();

    prepDebug();

    flat_shader->bind();
    glDisable(GL_DEPTH_TEST);

    GLuint viewProjectionLocation = glGetUniformLocation(
        flat_shader->getProgram(), "viewProjection");
    glUniformMatrix4fv(viewProjectionLocation, 1, false,
            glm::value_ptr(camViewProjection));

    debug_mesh->draw();

    glEnable(GL_DEPTH_TEST);
    flat_shader->unbind();

    if (time - debug_time > 1.0) {
        debug_time = time;

        std::vector<Contact> & contacts = system->getContacts();
        std::vector<std::shared_ptr<Body>> & bodies = system->getBodies();

        if (nFrames > 0) {
            frameTime /= nFrames;
            physicsTime /= nFrames;
        }
        else {
            frameTime = 0.0;
            physicsTime = 0.0;
        }

        frameTime *= 1000.0;
        physicsTime *= 1000.0;

        sprintf(debug_buff,
            "    Contacts: %ld\n"
            "      Bodies: %ld\n"
            "  Frame Time: %.02f ms\n"
            "Physics Time: %.02f ms\n"
            , contacts.size(), bodies.size(), frameTime, physicsTime);

        frameTime = 0;
        physicsTime = 0;
    }

    font->drawString(debug_buff, 10, 10);
    font->flush(width, height);

    //shadowTarget->blit(0, 0, 256, 256, 0);
}

void Demo::run() {
    init_demo();

    time = getTime();

    while (!glfwWindowShouldClose(window) && !close) {
        double frameStart = getTime();

        glfwPollEvents();

        double newTime = getTime();
        double elapsed = newTime - time;
        time = newTime;

        system->integrate(newTime, elapsed);
        physicsTime += getTime() - newTime;

        draw();

        glfwSwapBuffers(window);

        frameTime += getTime() - frameStart;
        nFrames++;
    }

    destroy_demo();
}

Demo::~Demo() {
    glfwDestroyWindow(window);

    glfwTerminate();
}

glm::vec3 transform(glm::mat4 mat, glm::vec3 vec) {
    // TODO
    glm::vec4 v4 = glm::vec4(vec.x, vec.y, vec.z, 1.0f);
    v4 = mat * v4;
    return glm::vec3(v4.x, v4.y, v4.z);
}

void Demo::prepDebug() {
    std::vector<MeshVertex> vertices;
    std::vector<int> indices;
    MeshVertex vert;

    for (auto & pair : meshes) {
        const float diff = 0.5f;
        int i0 = vertices.size();

        glm::mat4 world = pair.body->getTransform();

        vert.color = glm::vec4(1, 1, 0, 1);
        vert.position = transform(world, glm::vec3(-diff, 0, 0));
        vertices.push_back(vert);
        vert.color = glm::vec4(1, 1, 0, 1);
        vert.position = transform(world, glm::vec3(0, 0, 0));
        vertices.push_back(vert);

        vert.color = glm::vec4(1, 1, 0, 1);
        vert.position = transform(world, glm::vec3(0, -diff, 0));
        vertices.push_back(vert);
        vert.color = glm::vec4(1, 1, 0, 1);
        vert.position = transform(world, glm::vec3(0, 0, 0));
        vertices.push_back(vert);

        vert.color = glm::vec4(1, 1, 0, 1);
        vert.position = transform(world, glm::vec3(0, 0, -diff));
        vertices.push_back(vert);
        vert.color = glm::vec4(1, 1, 0, 1);
        vert.position = transform(world, glm::vec3(0, 0, 0));
        vertices.push_back(vert);

        vert.color = glm::vec4(1, 0, 0, 1);
        vert.position = transform(world, glm::vec3(diff, 0, 0));
        vertices.push_back(vert);
        vert.color = glm::vec4(1, 0, 0, 1);
        vert.position = transform(world, glm::vec3(0, 0, 0));
        vertices.push_back(vert);

        vert.color = glm::vec4(0, 1, 0, 1);
        vert.position = transform(world, glm::vec3(0, diff, 0));
        vertices.push_back(vert);
        vert.color = glm::vec4(0, 1, 0, 1);
        vert.position = transform(world, glm::vec3(0, 0, 0));
        vertices.push_back(vert);

        vert.color = glm::vec4(0, 0, 1, 1);
        vert.position = transform(world, glm::vec3(0, 0, diff));
        vertices.push_back(vert);
        vert.color = glm::vec4(0, 0, 1, 1);
        vert.position = transform(world, glm::vec3(0, 0, 0));
        vertices.push_back(vert);

        indices.push_back(i0 + 0);
        indices.push_back(i0 + 1);
        indices.push_back(i0 + 2);
        indices.push_back(i0 + 3);
        indices.push_back(i0 + 4);
        indices.push_back(i0 + 5);
        indices.push_back(i0 + 6);
        indices.push_back(i0 + 7);
        indices.push_back(i0 + 8);
        indices.push_back(i0 + 9);
        indices.push_back(i0 + 10);
        indices.push_back(i0 + 11);

        /*std::shared_ptr<CollisionShape> shape = pair.body->getCollisionShape();

        if (!shape)
            continue;

        i0 = vertices.size();
        vert.color = glm::vec4(1, 0, 0, 1);

        glm::vec3 min, max;
        shape->getBoundingBox(pair.body.get(), min, max);

        vert.position = glm::vec3(min.x, min.y, min.z);
        vertices.push_back(vert);
        vert.position = glm::vec3(max.x, min.y, min.z);
        vertices.push_back(vert);
        vert.position = glm::vec3(min.x, max.y, min.z);
        vertices.push_back(vert);
        vert.position = glm::vec3(max.x, max.y, min.z);
        vertices.push_back(vert);
        vert.position = glm::vec3(min.x, min.y, max.z);
        vertices.push_back(vert);
        vert.position = glm::vec3(max.x, min.y, max.z);
        vertices.push_back(vert);
        vert.position = glm::vec3(min.x, max.y, max.z);
        vertices.push_back(vert);
        vert.position = glm::vec3(max.x, max.y, max.z);
        vertices.push_back(vert);

        indices.push_back(i0 + 0);
        indices.push_back(i0 + 1);

        indices.push_back(i0 + 2);
        indices.push_back(i0 + 3);

        indices.push_back(i0 + 4);
        indices.push_back(i0 + 5);

        indices.push_back(i0 + 6);
        indices.push_back(i0 + 7);

        indices.push_back(i0 + 0);
        indices.push_back(i0 + 2);

        indices.push_back(i0 + 1);
        indices.push_back(i0 + 3);

        indices.push_back(i0 + 4);
        indices.push_back(i0 + 6);

        indices.push_back(i0 + 5);
        indices.push_back(i0 + 7);

        indices.push_back(i0 + 0);
        indices.push_back(i0 + 4);

        indices.push_back(i0 + 1);
        indices.push_back(i0 + 5);

        indices.push_back(i0 + 2);
        indices.push_back(i0 + 6);

        indices.push_back(i0 + 3);
        indices.push_back(i0 + 7);*/
    }

    std::vector<Contact> & contacts = system->getContacts();

    for (auto & contact : contacts) {
        const float diff = 0.25f;
        int i0 = vertices.size();

        vert.color = glm::vec4(0, 1, 0, 1);
        vert.position = contact.position + glm::vec3(-diff, 0, 0);
        vertices.push_back(vert);
        vert.color = glm::vec4(0, 1, 0, 1);
        vert.position = contact.position + glm::vec3(diff, 0, 0);
        vertices.push_back(vert);

        vert.color = glm::vec4(0, 1, 0, 1);
        vert.position = contact.position + glm::vec3(0, -diff, 0);
        vertices.push_back(vert);
        vert.color = glm::vec4(0, 1, 0, 1);
        vert.position = contact.position + glm::vec3(0, diff, 0);
        vertices.push_back(vert);

        vert.color = glm::vec4(0, 1, 0, 1);
        vert.position = contact.position + glm::vec3(0, 0, -diff);
        vertices.push_back(vert);
        vert.color = glm::vec4(0, 1, 0, 1);
        vert.position = contact.position + glm::vec3(0, 0, diff);
        vertices.push_back(vert);

        indices.push_back(i0 + 0);
        indices.push_back(i0 + 1);
        indices.push_back(i0 + 2);
        indices.push_back(i0 + 3);
        indices.push_back(i0 + 4);
        indices.push_back(i0 + 5);
    }

    debug_mesh->setVertices(&vertices[0], vertices.size());
    debug_mesh->setIndices(&indices[0], indices.size());
    debug_mesh->setMode(GL_LINES); // TODO redundant
}

}}
