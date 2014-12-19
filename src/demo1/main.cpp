/**
 * @file main.cpp
 *
 * @brief Demo 1 entrypoint
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <util/demo.h>
#include <util/planemesh.h>
#include <util/spheremesh.h>
#include <physics/springconstraint.h>
#include <physics/rodconstraint.h>
#include <physics/sphereshape.h>
#include <physics/planeshape.h>
#include <iostream>

using namespace Physics;
using namespace Physics::Util;

class Demo1 : public Demo {
private:

    std::vector<std::shared_ptr<Mesh>> meshes;

protected:

    // TODO: constraints that don't require extra bodies

    virtual void init_demo() override {
        std::shared_ptr<Shader> shader = getDefaultShader();
        std::shared_ptr<Camera> cam = getCamera();
        std::shared_ptr<System> system = getSystem();

        // Ground plane
        auto quadBody = std::make_shared<Body>();
        quadBody->setPosition(glm::vec3(0, 0, 0));
        quadBody->setFixed(true);
        quadBody->setCollisionShape(std::make_shared<PlaneShape>(glm::vec3(0, 1, 0), 0));
        meshes.push_back(std::make_shared<PlaneMesh>(shader, quadBody, 20, 10));
        system->addBody(quadBody);

        int N = 14;
        float R = 1.0f;

        float dy = sqrtf((R * 2.0f) * (R * 2.0f) - R * R);

        for (int j = 0; j < N; j++) {
            for (int i = 0; i < N - j; i++) {
                glm::vec3 p = glm::vec3(-(N - j - 1) * R + i * 2.0f * R, j * dy + R, 0.0f);

                auto sphereBody = std::make_shared<Body>();
                system->addBody(sphereBody);
                sphereBody->setPosition(p);
                sphereBody->setCollisionShape(std::make_shared<SphereShape>(R));

                if (j == 0 && (i == 0 || i == N - j - 1))
                    sphereBody->setFixed(true);

                meshes.push_back(std::make_shared<SphereMesh>(shader, sphereBody, 30, 15, 1));
            }
        }

        cam->setPosition(glm::vec3(10, 20, 40));
        cam->setTarget(glm::vec3(0, N * R, 0));
    }

    virtual void demo_mouseDown(int button) {
        std::shared_ptr<Camera> cam = getCamera();

        auto sphereBody = std::make_shared<Body>();
        getSystem()->addBody(sphereBody);
        sphereBody->setPosition(cam->getPosition());
        sphereBody->setVelocity(glm::normalize(cam->getTarget() - cam->getPosition()) * 100.0f);
        sphereBody->setCollisionShape(std::make_shared<SphereShape>(3.0f));
        sphereBody->setMass(5.0f);
        meshes.push_back(std::make_shared<SphereMesh>(getDefaultShader(), sphereBody, 30, 15, 3.0f));
    }

    virtual void draw_demo() override {
        std::shared_ptr<Camera> cam = getCamera();

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        for (auto mesh : meshes)
            mesh->draw(cam->getViewProjection());

        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    virtual void destroy_demo() override {
    }

};

int main(int argc, char *argv[]) {
    Demo1 demo1;
    demo1.run();
}
