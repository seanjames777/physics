/**
 * @file demo.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <cassert>
#include <functional>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <util/texture.h>
#include <util/rendertarget.h>
#include <util/shader.h>
#include <util/camera.h>
#include <util/mesh.h>
#include <iostream>
#include <physics/collision/shape.h>
#include <physics/dynamics/body.h>
#include <physics/system.h>
#include <util/demo.h>

#ifndef WIN32
#include <sys/time.h>
#include <unistd.h>
#else
#include <Windows.h>
#endif

using namespace std::placeholders;

namespace Physics { namespace Util {

double Demo::getTime() {
#ifdef WIN32
	LARGE_INTEGER time, frequency;
	QueryPerformanceCounter(&time);
	QueryPerformanceFrequency(&frequency);

	return (double)time.QuadPart / (double)frequency.QuadPart;
#else
	struct timeval now;
	gettimeofday(&now, nullptr);
	double time = 0.0;

	time += now.tv_sec;
	time += now.tv_usec / 1000000.0;

	return time;
#endif
}

void Demo::resizeHandler(GLFWwindow *window, int width, int height) {
    Demo *demo = (Demo *)glfwGetWindowUserPointer(window);

    demo->width = width;
    demo->height = height;

    glViewport(0, 0, width, height);

    demo->camera->setAspectRatio((float)width / (float)height);
}

void Demo::mouseButtonHandler(GLFWwindow *window, int button, int action, int mods) {
    Demo *demo = (Demo *)glfwGetWindowUserPointer(window);

    int mask = 1 << button;

    if (action == GLFW_PRESS) {
        demo->mouseButtons |= mask;
        demo->demo_mouseDown(button);
    }
    else if (action == GLFW_RELEASE) {
        demo->mouseButtons &= ~mask;
    }
    // TODO else {}
}

void Demo::mouseMoveHandler(GLFWwindow *window, double x, double y) {
    Demo *demo = (Demo *)glfwGetWindowUserPointer(window);

    const float sensitivity = 0.0075f;
    float dx = x - demo->mouseX;
    float dy = y - demo->mouseY;

    // TODO should just load at beginning. same for buttons.
    if (demo->mouseX != 0 && demo->mouseY != 0 && (demo->mouseButtons & (1 << 0))) {
        demo->camera->setYaw(demo->camera->getYaw() - dx * sensitivity);
        demo->camera->setPitch(demo->camera->getPitch() + dy * sensitivity);
    }

    demo->mouseX = x;
    demo->mouseY = y;
}

void Demo::keyHandler(GLFWwindow *window, int key, int scanCode, int action, int mods) {
    Demo *demo = (Demo *)glfwGetWindowUserPointer(window);

    int bucket = key / sizeof(int);
    int idx = key % sizeof(int);

    if (action == GLFW_PRESS) {
        demo->keys[bucket] |= (1 << idx);

        if (key == GLFW_KEY_SPACE) {
            demo->pausePhysics = !demo->pausePhysics;
            demo->system->setTimeWarp(demo->pausePhysics ? 0.0 : demo->timeWarp);
        }
        else if (key == GLFW_KEY_LEFT_BRACKET || key == GLFW_KEY_RIGHT_BRACKET) {
            demo->timeWarp += (key == GLFW_KEY_LEFT_BRACKET ? -0.05 : 0.05);
            demo->system->setTimeWarp(demo->pausePhysics ? 0.0 : demo->timeWarp);
        }
        else if (key == GLFW_KEY_P)
            demo->wireframe = !demo->wireframe;
    }
    else if (action == GLFW_RELEASE) {
        demo->keys[bucket] &= ~(1 << idx);
    }
}

bool Demo::isKeyDown(int key) {
    int bucket = key / sizeof(int);
    int idx = key % sizeof(int);

    return (keys[bucket] & (1 << idx)) ? true : false;
}

std::shared_ptr<FPSCamera> Demo::getCamera() {
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

Demo::Demo(char *executable, std::string title, int width, int height, int shadowSize, bool vsync)
    : width(width),
      height(height),
      shadowSize(shadowSize),
      window(nullptr),
      close(false),
      phong_shader(nullptr),
      shadow_shader(nullptr),
      font(nullptr),
      texture(nullptr),
      camera(nullptr),
      system(nullptr),
      time(0.0),
      debug_time(0.0),
      shadowBounds(50.0f),
      shadowNear(-50.0f),
      shadowFar(50.0f),
      shadowTarget(nullptr),
      frameTime(0.0),
      physicsTime(0.0),
      contacts(0),
      avgFrameTime(0.0),
      avgPhysicsTime(0.0),
      avgContacts(0),
      nFrames(0),
      mouseX(0),
      mouseY(0),
      mouseButtons(0),
      timeWarp(1.0),
      pausePhysics(false),
      wireframe(false)
{
#ifndef WIN32
    #define BUFFSZ 512
    char buffer[BUFFSZ];
    getcwd(buffer, BUFFSZ);
    strcat(buffer, "/");
    memcpy(strlen(buffer) + buffer, executable, strlen(executable) + 1);

    int i = 0, j;
    for (j = 0; buffer[j] != '\0'; j++)
        if (buffer[j] == '/')
            i = j;

    buffer[i+1] = '\0';

    std::cout << buffer << std::endl;

    chdir(buffer);
#endif

    glfwInit();

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_SRGB_CAPABLE, true);

    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    glfwMakeContextCurrent(window);

    glfwSetWindowUserPointer(window, this);
    glfwSetWindowSizeCallback(window, resizeHandler);
    glfwSetMouseButtonCallback(window, mouseButtonHandler);
    glfwSetCursorPosCallback(window, mouseMoveHandler);
    glfwSetKeyCallback(window, keyHandler);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glewExperimental = true;
    glewInit();
    glGetError(); // Eat spurious error

    printf("OpenGL Renderer\n");
    printf("Vendor:       %s\n", glGetString(GL_VENDOR));
    printf("Renderer:     %s\n", glGetString(GL_RENDERER));
    printf("Version:      %s\n", glGetString(GL_VERSION));
    printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    int num_extensions;
    glGetIntegerv(GL_NUM_EXTENSIONS, &num_extensions);
    printf("Extensions: %d\n", num_extensions);

    for (int i = 0; i < num_extensions; i++)
        printf("  %s\n", glGetStringi(GL_EXTENSIONS, i));

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE); // TODO
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_FRAMEBUFFER_SRGB);

    glfwSwapInterval(vsync ? 1 : 0);

    phong_shader = std::make_shared<Shader>();
    phong_shader->setStage(ShaderStageVS, "content/shaders/phong.vs");
    phong_shader->setStage(ShaderStageFS, "content/shaders/phong.fs");
    phong_shader->finish();
    assert(!phong_shader->error());

    phong_shader->setUniformBufferBinding("SceneUniforms", 0);
    phong_shader->setUniformBufferBinding("MeshUniforms", 1);
    phong_shader->setTextureBinding("diffuseTexture", 0);
    phong_shader->setTextureBinding("depthTexture", 1);

    shadow_shader = std::make_shared<Shader>();
    shadow_shader->setStage(ShaderStageVS, "content/shaders/shadow.vs");
    shadow_shader->setStage(ShaderStageFS, "content/shaders/shadow.fs");
    shadow_shader->finish();
    assert(!shadow_shader->error());

    shadow_shader->setUniformBufferBinding("SceneUniforms", 0);
    shadow_shader->setUniformBufferBinding("MeshUniforms", 1);
    shadow_shader->setTextureBinding("diffuseTexture", 0);
    shadow_shader->setTextureBinding("depthTexture", 1);

    flat_shader = std::make_shared<Shader>();
    flat_shader->setStage(ShaderStageVS, "content/shaders/flat.vs");
    flat_shader->setStage(ShaderStageFS, "content/shaders/flat.fs");
    flat_shader->finish();
    assert(!flat_shader->error());

    flat_shader->setUniformBufferBinding("SceneUniforms", 0);
    flat_shader->setUniformBufferBinding("MeshUniforms", 1);
    flat_shader->setTextureBinding("diffuseTexture", 0);
    flat_shader->setTextureBinding("depthTexture", 1);

    camera = std::make_shared<FPSCamera>(glm::vec3(), 0, 0);
    camera->setAspectRatio((float)width / (float)height);

    system = std::make_shared<System>();

    // TODO: Reuse the depth buffer instead of a texture
    // TODO: Renderbuffer may be faster for some things
    // TODO format, filter
    shadowTex = std::make_shared<Texture>(
        SurfaceFormatR16F, shadowSize, shadowSize, 1, TextureFilterModeBilinear, 0.0f,
        TextureAddressModeClamp, TextureAddressModeClamp);

    std::shared_ptr<RenderBuffer> shadowDepthBuffer = std::make_shared<RenderBuffer>(
        SurfaceFormatDepth24, shadowSize, shadowSize);

    shadowTarget = std::make_shared<RenderTarget>();
    shadowTarget->addColorTarget(shadowTex);
    shadowTarget->addDepthStencilTarget(shadowDepthBuffer);
    shadowTarget->finish();

    debug_mesh = std::make_shared<Mesh>(BufferUsageStream);

    // TODO
    //font = std::make_shared<Font>("content/fonts/consolas_bold.ttf", 0, 12);

    texture = Texture::fromJpeg("content/textures/checker.jpg", 6, TextureFilterModeBilinear,
         0.0f, TextureAddressModeWrap, TextureAddressModeWrap);

    memset(keys, 0, sizeof(keys));

    uniformBuffer = std::make_shared<Buffer>(BufferBindingUniformBuffer, BufferUsageStream);

    glm::vec3 lightDir = glm::normalize(glm::vec3(0.8f, 1.0f, 0.6f));
    glm::mat4 shadowView = glm::lookAt(lightDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::mat4 shadowProj = glm::ortho(
        -shadowBounds, shadowBounds, -shadowBounds, shadowBounds, shadowNear, shadowFar);
    sceneUniforms.lightViewProjection = shadowProj * shadowView;

    sceneUniforms.lightDirection = lightDir;
    sceneUniforms.depthTextureSize = shadowSize;

    CHECK_GL_ERROR();
}

void Demo::updateDebugBuff() {
    // TODO changes quickly potentially
    std::vector<System::ContactEx> & contacts_vec = system->getContacts();
    std::vector<std::shared_ptr<Body>> & bodies = system->getBodies();

    contacts += contacts_vec.size();

    if (time - debug_time > 1.0) {
        debug_time = time;

        if (nFrames > 0) {
            frameTime /= nFrames;
            physicsTime /= nFrames;
            contacts /= nFrames;
        }
        else {
            frameTime = 0.0;
            physicsTime = 0.0;
            contacts = 0;
        }

        avgFrameTime = frameTime * 1000.0;
        avgPhysicsTime = physicsTime * 1000.0;
        avgContacts = contacts;

        frameTime = 0;
        physicsTime = 0;
        nFrames = 0;
    }

    sprintf(debug_buff,
        "    Contacts: %d\n"
        "      Bodies: %ld\n"
        "  Frame Time: %.02f ms\n"
        "Physics Time: %.02f ms\n"
        "   Time Warp: %.02f\n"
        "%s"
        , avgContacts, bodies.size(), avgFrameTime, avgPhysicsTime,
        timeWarp, pausePhysics ? "Simulation paused\n" : "");
}

void Demo::draw() {
    for (auto & pair : meshes) {
        MeshUniforms *uniforms = pair.mesh->getUniforms();

        glm::mat4 world = pair.body->getLocalToWorld();
        uniforms->world = world;
        uniforms->worldInverseTranspose = glm::inverse(glm::transpose(world));

        pair.mesh->flushUniforms();
    }

    sceneUniforms.viewProjection = camera->getViewProjection();
    uniformBuffer->setData(sizeof(SceneUniforms), &sceneUniforms);
    uniformBuffer->bind(0);

    shadow_shader->bind();

    shadowTarget->bind();

    glViewport(0, 0, shadowSize, shadowSize);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto & pair : meshes)
        pair.mesh->draw(1);

    shadow_shader->unbind();
    shadowTarget->unbind();

    glViewport(0, 0, width, height);

    glClearColor(0.0289f, 0.133f, 0.612f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    phong_shader->bind();

    texture->bind(0);
    shadowTex->bind(1);

    if (wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    for (auto & pair : meshes)
        pair.mesh->draw(1);

    if (wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    texture->unbind(0);
    shadowTex->unbind(1);

    phong_shader->unbind();

    //prepDebug();

    //flat_shader->bind();
    //glDisable(GL_DEPTH_TEST);

    // TODO: Uniforms
    //debug_mesh->draw(1);

    //glEnable(GL_DEPTH_TEST);
    //flat_shader->unbind();

    //updateDebugBuff();

    //font->drawString(debug_buff, 10, 10);
    //font->flush(width, height);

    uniformBuffer->unbind(0);

    //shadowTarget->blit(0, 0, 300, 300, 0);
}

void Demo::updateCamera(double dt) {
    glm::vec3 local;

    if (isKeyDown(GLFW_KEY_W))
        local += glm::vec3(0, 0, 1);
    if (isKeyDown(GLFW_KEY_S))
        local += glm::vec3(0, 0, -1);
    if (isKeyDown(GLFW_KEY_A))
        local += glm::vec3(-1, 0, 0);
    if (isKeyDown(GLFW_KEY_D))
        local += glm::vec3(1, 0, 0);

    glm::vec3 global =
        local.x * camera->getRight() +
        local.z * camera->getForward();

    const float camSpeed = 40.0f * dt;

    camera->setPosition(camSpeed * global + camera->getPosition());
}

void Demo::run() {
    init_demo();

    time = getTime();

    #define GRAPHICS_FRAMES 200
    static double graphicsTime[GRAPHICS_FRAMES];

    while (!glfwWindowShouldClose(window) && !close) {
        double newTime = getTime();
        double elapsed = newTime - time;
        time = newTime;

        glfwPollEvents();

        system->integrate(newTime, elapsed);

        double elapsedPhysics = getTime() - newTime;
        physicsTime += elapsedPhysics;

        updateCamera(elapsed);

        double graphicsStart = getTime();
        draw();

        glFinish();
        glfwSwapBuffers(window);

        double elapsedGraphics = getTime() - graphicsStart;

        graphicsTime[nFrames % GRAPHICS_FRAMES] = elapsedGraphics;

        if (nFrames % GRAPHICS_FRAMES == 0) {
            double avgGraphics = 0.0;

            for (int i = 0; i < GRAPHICS_FRAMES; i++)
                avgGraphics += graphicsTime[i];

            avgGraphics /= GRAPHICS_FRAMES;

            //printf("Graphics: %f\n", avgGraphics * 1000.0);
        }

        frameTime += getTime() - newTime;
        nFrames++;

        /*printf("physics:  %fms\n"
               "graphics: %fms\n",
               elapsedPhysics * 1000.0,
               elapsedGraphics * 1000.0);*/
    }

    destroy_demo();
}

Demo::~Demo() {
    glfwDestroyWindow(window);

    glfwTerminate();
}

void Demo::prepDebug() {
    std::vector<MeshVertex> vertices;
    std::vector<unsigned int> indices;
    MeshVertex vert;

    for (auto & pair : meshes) {
        const float diff = 0.5f;
        unsigned int i0 = vertices.size();

        glm::mat4 world = pair.body->getLocalToWorld();

        vert.color = glm::vec4(1, 1, 0, 1);
        vert.position = glm::vec3(world * glm::vec4(-diff, 0, 0, 1));
        vertices.push_back(vert);
        vert.color = glm::vec4(1, 1, 0, 1);
        vert.position = glm::vec3(world * glm::vec4(0, 0, 0, 1));
        vertices.push_back(vert);

        vert.color = glm::vec4(1, 1, 0, 1);
        vert.position = glm::vec3(world * glm::vec4(0, -diff, 0, 1));
        vertices.push_back(vert);
        vert.color = glm::vec4(1, 1, 0, 1);
        vert.position = glm::vec3(world * glm::vec4(0, 0, 0, 1));
        vertices.push_back(vert);

        vert.color = glm::vec4(1, 1, 0, 1);
        vert.position = glm::vec3(world * glm::vec4(0, 0, -diff, 1));
        vertices.push_back(vert);
        vert.color = glm::vec4(1, 1, 0, 1);
        vert.position = glm::vec3(world * glm::vec4(0, 0, 0, 1));
        vertices.push_back(vert);

        vert.color = glm::vec4(1, 0, 0, 1);
        vert.position = glm::vec3(world * glm::vec4(diff, 0, 0, 1));
        vertices.push_back(vert);
        vert.color = glm::vec4(1, 0, 0, 1);
        vert.position = glm::vec3(world * glm::vec4(0, 0, 0, 1));
        vertices.push_back(vert);

        vert.color = glm::vec4(0, 1, 0, 1);
        vert.position = glm::vec3(world * glm::vec4(0, diff, 0, 1));
        vertices.push_back(vert);
        vert.color = glm::vec4(0, 1, 0, 1);
        vert.position = glm::vec3(world * glm::vec4(0, 0, 0, 1));
        vertices.push_back(vert);

        vert.color = glm::vec4(0, 0, 1, 1);
        vert.position = glm::vec3(world * glm::vec4(0, 0, diff, 1));
        vertices.push_back(vert);
        vert.color = glm::vec4(0, 0, 1, 1);
        vert.position = glm::vec3(world * glm::vec4(0, 0, 0, 1));
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

        /*std::shared_ptr<Shape> shape = pair.body->getShape();

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

    std::vector<System::ContactEx> & contacts = system->getContacts();

    for (auto & contact : contacts) {
        const float diff = 0.25f;
        unsigned int i0 = vertices.size();

        glm::vec3 p = contact.contact.position;

        glm::vec4 color = glm::vec4(1, 0, 0, 1);

        vert.color = color;
        vert.position = p + glm::vec3(-diff, 0, 0);
        vertices.push_back(vert);
        vert.position = p + glm::vec3(diff, 0, 0);
        vertices.push_back(vert);

        vert.position = p + glm::vec3(0, -diff, 0);
        vertices.push_back(vert);
        vert.position = p + glm::vec3(0, diff, 0);
        vertices.push_back(vert);

        vert.position = p + glm::vec3(0, 0, -diff);
        vertices.push_back(vert);
        vert.position = p + glm::vec3(0, 0, diff);
        vertices.push_back(vert);

        vert.position = p;
        vertices.push_back(vert);
        vert.position = p + contact.contact.normal * 2.0f;
        vertices.push_back(vert);

        indices.push_back(i0 + 0);
        indices.push_back(i0 + 1);
        indices.push_back(i0 + 2);
        indices.push_back(i0 + 3);
        indices.push_back(i0 + 4);
        indices.push_back(i0 + 5);
        //indices.push_back(i0 + 6);
        //indices.push_back(i0 + 7);
    }

    debug_mesh->setVertices(&vertices[0], vertices.size());
    debug_mesh->setIndices(&indices[0], indices.size());
    debug_mesh->setRenderMode(MeshRenderModeLines); // TODO redundant
}

}}
