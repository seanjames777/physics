cmake_minimum_required(VERSION 2.6)
project(physics)

include_directories(include/)

set(CMAKE_CXX_FLAGS "-std=c++11 -fno-exceptions -O3")

add_library(physics
    src/physics/body.cpp
    src/physics/system.cpp
    src/physics/constraint.cpp
    src/physics/rodconstraint.cpp
    src/physics/springconstraint.cpp
    src/physics/collisionshape.cpp
    src/physics/sphereshape.cpp
    src/physics/planeshape.cpp
)

add_library(util
    src/util/camera.cpp
    src/util/shader.cpp
    src/util/rendertarget.cpp
    src/util/mesh.cpp
    src/util/planemesh.cpp
    src/util/spheremesh.cpp
    src/util/cubemesh.cpp
    src/util/demo.cpp
)

add_executable(demo1
    src/demo1/main.cpp
)

target_link_libraries(demo1
    physics
    util
    -framework\ OpenGL
    -lglew
    -lglfw
)

install(TARGETS demo1 DESTINATION bin/)

install(FILES content/shaders/phong.vs DESTINATION bin/content/shaders/)
install(FILES content/shaders/phong.fs DESTINATION bin/content/shaders/)
install(FILES content/shaders/shadow.vs DESTINATION bin/content/shaders/)
install(FILES content/shaders/shadow.fs DESTINATION bin/content/shaders/)
install(FILES content/shaders/flat.vs DESTINATION bin/content/shaders/)
install(FILES content/shaders/flat.fs DESTINATION bin/content/shaders/)
