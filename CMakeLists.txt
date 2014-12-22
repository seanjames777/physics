cmake_minimum_required(VERSION 2.6)
project(physics)

include_directories(include/)

set(CMAKE_CXX_FLAGS "-std=c++11 -fno-exceptions -O3")
set(CMAKE_CXX_FLAGS "-I/opt/local/include/freetype2/ ${CMAKE_CXX_FLAGS}")

add_library(physics
    src/physics/collision/planeshape.cpp
    src/physics/collision/shape.cpp
    src/physics/collision/sphereshape.cpp
    src/physics/constraints/constraint.cpp
    src/physics/constraints/rodconstraint.cpp
    src/physics/constraints/springconstraint.cpp
    src/physics/dynamics/body.cpp
    src/physics/system.cpp
)

add_library(util
    src/util/demo.cpp
    src/util/graphics/camera.cpp
    src/util/graphics/cubemesh.cpp
    src/util/graphics/font.cpp
    src/util/graphics/fpscamera.cpp
    src/util/graphics/mesh.cpp
    src/util/graphics/planemesh.cpp
    src/util/graphics/rendertarget.cpp
    src/util/graphics/shader.cpp
    src/util/graphics/spheremesh.cpp
)

target_link_libraries(util
    -framework\ OpenGL
    -lglew
    -lglfw
    -L/opt/local/lib/
    -lfreetype
)

add_executable(demo1 src/demos/demo1.cpp)
target_link_libraries(demo1 physics util)
install(TARGETS demo1 DESTINATION bin/)

add_executable(demo2 src/demos/demo2.cpp)
target_link_libraries(demo2 physics util)
install(TARGETS demo2 DESTINATION bin/)

install(FILES content/shaders/phong.vs DESTINATION bin/content/shaders/)
install(FILES content/shaders/phong.fs DESTINATION bin/content/shaders/)
install(FILES content/shaders/shadow.vs DESTINATION bin/content/shaders/)
install(FILES content/shaders/shadow.fs DESTINATION bin/content/shaders/)
install(FILES content/shaders/flat.vs DESTINATION bin/content/shaders/)
install(FILES content/shaders/flat.fs DESTINATION bin/content/shaders/)
install(FILES content/shaders/font.vs DESTINATION bin/content/shaders/)
install(FILES content/shaders/font.fs DESTINATION bin/content/shaders/)
install(FILES content/fonts/consolas_bold.ttf DESTINATION bin/content/fonts/)
