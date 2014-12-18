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

using namespace Physics;
using namespace Physics::Util;

class Demo1 : public Demo {
private:

    std::shared_ptr<Mesh> quad, sphere;
    std::shared_ptr<Body> quadBody, sphereBody;
    std::shared_ptr<SpringConstraint> spring;

protected:

    virtual void init_demo() override {
        std::shared_ptr<Shader> shader = getDefaultShader();
        std::shared_ptr<Camera> cam = getCamera();
        std::shared_ptr<System> system = getSystem();

        quadBody = std::make_shared<Body>();
        sphereBody = std::make_shared<Body>();
        system->addBody(quadBody);
        system->addBody(sphereBody);

        quadBody->setFixed(true);
        sphereBody->setPosition(glm::vec3(-7.0f, 0.0f, 0));

        quad = std::make_shared<PlaneMesh>(shader, quadBody);
        sphere = std::make_shared<SphereMesh>(shader, sphereBody);

        spring = std::make_shared<SpringConstraint>(quadBody, sphereBody, 4.0f, 7.0f, 0.01f);
        system->addConstraint(spring);

        cam->setPosition(glm::vec3(0, -10, 30));
        cam->setTarget(glm::vec3(0, -10, 0));
    }

    virtual void draw_demo() override {
        std::shared_ptr<Camera> cam = getCamera();

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        quad->draw(cam->getViewProjection());
        sphere->draw(cam->getViewProjection());

        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    virtual void destroy_demo() override {
    }

};

int main(int argc, char *argv[]) {
    Demo1 demo1;
    demo1.run();
}
