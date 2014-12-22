/**
 * @file main.cpp
 *
 * @brief Demo 1 entrypoint
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <util/demo.h>
#include <util/graphics/planemesh.h>
#include <util/graphics/spheremesh.h>
#include <util/graphics/cubemesh.h>
#include <util/graphics/fpscamera.h>
#include <physics/constraints/springconstraint.h>
#include <physics/constraints/rodconstraint.h>
#include <physics/collision/sphereshape.h>
#include <physics/collision/planeshape.h>
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

    virtual void init_demo() override {
        std::shared_ptr<FPSCamera> cam = getCamera();
        std::shared_ptr<System> system = getSystem();

        // Ground plane
        auto quadBody = std::make_shared<Body>();
        quadBody->setPosition(glm::vec3(0, 0, 0));
        quadBody->setFixed(true);
        quadBody->setCollisionShape(std::make_shared<PlaneShape>(glm::vec3(0, 1, 0), 0));
        addMesh(std::make_shared<PlaneMesh>(40, 40), quadBody);
        system->addBody(quadBody);

        int N = 15;
        float R = 1.0f;
        float M = 1.0f;

        float dy = sqrtf((R * 2.0f) * (R * 2.0f) - R * R);

        for (int j = 0; j < N; j++) {
            for (int i = 0; i < N - j; i++) {
                glm::vec3 p = glm::vec3(-(N - j - 1) * R + i * 2.0f * R, j * dy + R, 0.0f);

                auto sphereBody = std::make_shared<Body>();
                system->addBody(sphereBody);
                sphereBody->setPosition(p);
                sphereBody->setCollisionShape(std::make_shared<SphereShape>(R));
                sphereBody->setMass(M);

                float I = 2.0f * M * R * R / 5.0f;
                sphereBody->setInertiaTensor(glm::mat3(I, 0, 0, 0, I, 0, 0, 0, I));

                if (j == 0 && (i == 0 || i == N - j - 1))
                    sphereBody->setFixed(true);

                addMesh(std::make_shared<SphereMesh>(30, 15, 1.0f), sphereBody);
            }
        }

        cam->setPosition(glm::vec3(10, 20, 40));
        cam->setYaw(193);
        cam->setPitch(10);
    }

    virtual void demo_mouseDown(int button) {
        if (button == 1) {
            std::shared_ptr<Camera> cam = getCamera();

            auto sphereBody = std::make_shared<Body>();
            getSystem()->addBody(sphereBody);
            sphereBody->setPosition(cam->getPosition());
            sphereBody->setLinearVelocity(glm::normalize(cam->getTarget() - cam->getPosition()) * 100.0f);
            sphereBody->setCollisionShape(std::make_shared<SphereShape>(3.0f));
            sphereBody->setMass(5.0f);
            float I = 2.0f * 5.0f * 3.0f * 3.0f / 5.0f;
            sphereBody->setInertiaTensor(glm::mat3(I, 0, 0, 0, I, 0, 0, 0, I));
            addMesh(std::make_shared<SphereMesh>(30, 15, 3.0f), sphereBody);
        }
    }

    virtual void destroy_demo() override {
    }

};

int main(int argc, char *argv[]) {
    Demo1 demo1;
    demo1.run();
}
