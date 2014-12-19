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
#include <util/camera.h>
#include <physics/springconstraint.h>
#include <physics/rodconstraint.h>
#include <physics/sphereshape.h>
#include <physics/planeshape.h>
#include <physics/system.h>
#include <iostream>

using namespace Physics;
using namespace Physics::Util;

class Demo1 : public Demo {
public:

    Demo1()
        : Demo("Stacking Demo")
    {
    }

protected:

    // TODO: constraints that don't require extra bodies

    virtual void init_demo() override {
        std::shared_ptr<Camera> cam = getCamera();
        std::shared_ptr<System> system = getSystem();

        // Ground plane
        auto quadBody = std::make_shared<Body>();
        quadBody->setPosition(glm::vec3(0, 0, 0));
        quadBody->setFixed(true);
        quadBody->setCollisionShape(std::make_shared<PlaneShape>(glm::vec3(0, 1, 0), 0));
        addMesh(std::make_shared<PlaneMesh>(quadBody, 20, 10));
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

                addMesh(std::make_shared<SphereMesh>(sphereBody, 30, 15, 1));
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
        addMesh(std::make_shared<SphereMesh>(sphereBody, 30, 15, 3.0f));
    }

    virtual void destroy_demo() override {
    }

};

int main(int argc, char *argv[]) {
    Demo1 demo1;
    demo1.run();
}
