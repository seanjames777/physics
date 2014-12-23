/**
 * @file main.cpp
 *
 * @brief Demo 1 entrypoint
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <util/demo.h>
#include <graphics/planemesh.h>
#include <graphics/spheremesh.h>
#include <graphics/cubemesh.h>
#include <graphics/fpscamera.h>
#include <physics/constraints/springconstraint.h>
#include <physics/constraints/rodconstraint.h>
#include <physics/collision/sphereshape.h>
#include <physics/collision/planeshape.h>
#include <physics/system.h>
#include <iostream>

using namespace Physics;
using namespace Physics::Util;

class Demo2 : public Demo {
public:

    Demo2()
        : Demo("Box Demo")
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
        quadBody->setShape(std::make_shared<PlaneShape>(glm::vec3(0, 1, 0), 0));
        addMesh(std::make_shared<PlaneMesh>(40, 40), quadBody);
        system->addBody(quadBody);

        cam->setPosition(glm::vec3(10, 20, 40));
        cam->setYaw(193);
        cam->setPitch(10);

        auto cubeBody = std::make_shared<Body>();
        getSystem()->addBody(cubeBody);
        cubeBody->setPosition(glm::vec3(0, 2, 5));
        cubeBody->setShape(std::make_shared<SphereShape>(2.0f));
        cubeBody->setMass(1.0f);
        float I = 2.0f / 3.0f;
        cubeBody->setInertiaTensor(glm::mat3(I, 0, 0, 0, I, 0, 0, 0, I));
        addMesh(std::make_shared<CubeMesh>(2, 2, 2), cubeBody);

        //cubeBody->addImpulse(glm::vec3(-8, 0, 0), glm::vec3(0, 1.0f, -1));
        //cubeBody->addImpulse(glm::vec3( 8, 0, 0), glm::vec3(0, 1.0f,  1));
        //cubeBody->addImpulse(glm::vec3( 0, 0, 10), glm::vec3(0, 2.0f,  0));
    }

    virtual void demo_mouseDown(int button) {
        if (button == 1) {
            std::shared_ptr<Camera> cam = getCamera();

            auto cubeBody = std::make_shared<Body>();
            getSystem()->addBody(cubeBody);
            cubeBody->setPosition(cam->getPosition());
            cubeBody->setLinearVelocity(glm::normalize(cam->getTarget() - cam->getPosition()) * 60.0f);
            cubeBody->setShape(std::make_shared<SphereShape>(2.0f));
            cubeBody->setMass(3.0f);
            float I = 2.0f / 3.0f;
            cubeBody->setInertiaTensor(glm::mat3(I, 0, 0, 0, I, 0, 0, 0, I));
            addMesh(std::make_shared<CubeMesh>(2, 2, 2), cubeBody);
        }
    }

    virtual void destroy_demo() override {
    }

};

int main(int argc, char *argv[]) {
    Demo2 demo2;
    demo2.run();
}
