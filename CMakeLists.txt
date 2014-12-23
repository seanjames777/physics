cmake_minimum_required(VERSION 2.6)
project(physics)

include_directories(include/)

set(CMAKE_CXX_FLAGS "-std=c++11 -fno-exceptions -O3")
set(CMAKE_CXX_FLAGS "-I/usr/include -I/usr/local/include/ -I/opt/local/include/ -I/opt/local/include/freetype2/ ${CMAKE_CXX_FLAGS}")

add_library(physics
    src/physics/collision/collision.cpp
    src/physics/collision/planeshape.cpp
    src/physics/collision/shape.cpp
    src/physics/collision/sphereshape.cpp
    src/physics/constraints/constraint.cpp
    src/physics/constraints/rodconstraint.cpp
    src/physics/constraints/springconstraint.cpp
    src/physics/dynamics/body.cpp
    src/physics/system.cpp
    src/physics/transform.cpp
)

add_library(util
    src/util/demo.cpp
    src/graphics/camera.cpp
    src/graphics/cubemesh.cpp
    src/graphics/font.cpp
    src/graphics/fpscamera.cpp
    src/graphics/mesh.cpp
    src/graphics/path.cpp
    src/graphics/planemesh.cpp
    src/graphics/rendertarget.cpp
    src/graphics/shader.cpp
    src/graphics/spheremesh.cpp
    src/graphics/texture.cpp
    src/graphics/textureutil.cpp
)

target_link_libraries(util
    -framework\ OpenGL
    -lglew
    -lglfw
    -L/opt/local/lib/
    -lfreetype
    -lpng
    -ljpeg
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
install(FILES content/textures/checker.jpg DESTINATION bin/content/textures/)
