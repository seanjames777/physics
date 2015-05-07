cmake_minimum_required(VERSION 2.6)
project(physics)

include_directories(include/)

if (NOT "${CMAKE_SYSTEM}" MATCHES "Windows")
    set(CMAKE_CXX_FLAGS "-std=c++11 -fno-exceptions -fno-rtti -O3 -g -DDEBUG")
    set(CMAKE_CXX_FLAGS "-I/usr/include -I/usr/local/include/ -I/opt/local/include/ -I/opt/local/include/freetype2/ ${CMAKE_CXX_FLAGS}")
endif()

set(CMAKE_INSTALL_RPATH "\$ORIGIN/")
set(CMAKE_INSTALL_NAME_DIR "@executable_path/")

add_library(physics SHARED
    src/physics/collision/collision.cpp
    src/physics/collision/cubeshape.cpp
    src/physics/collision/planeshape.cpp
    src/physics/collision/shape.cpp
    src/physics/collision/sphereshape.cpp
    src/physics/constraints/constraint.cpp
    src/physics/constraints/rodconstraint.cpp
    src/physics/constraints/springconstraint.cpp
    src/physics/dynamics/body.cpp
    src/physics/system.cpp
    src/physics/transform.cpp

    include/physics/collision/collision.h
    include/physics/collision/cubeshape.h
    include/physics/collision/planeshape.h
    include/physics/collision/shape.h
    include/physics/collision/sphereshape.h
    include/physics/constraints/constraint.h
    include/physics/constraints/rodconstraint.h
    include/physics/constraints/springconstraint.h
    include/physics/dynamics/body.h
    include/physics/system.h
    include/physics/transform.h
    include/physics/defs.h
)

add_library(util SHARED
    src/util/geometrybuilder.cpp
    src/util/mesh.cpp
    src/util/camera.cpp
    src/util/buffer.cpp
    src/util/texture.cpp
    src/util/rendertarget.cpp
    src/util/shader.cpp
    src/util/demo.cpp

    include/util/buffer.h
    include/util/camera.h
    include/util/demo.h
    include/util/geometrybuilder.h
    include/util/defs.h
    include/util/mesh.h
    include/util/rendertarget.h
    include/util/shader.h
    include/util/texture.h
    include/util/defs.h
)

add_library(glfw SHARED IMPORTED)
add_library(glew SHARED IMPORTED)
add_library(jpeg SHARED IMPORTED)

if ("${CMAKE_SYSTEM}" MATCHES "Windows")
    include_directories(prebuilt_windows/include/)

    set_target_properties(glew PROPERTIES IMPORTED_IMPLIB_DEBUG ${CMAKE_SOURCE_DIR}/prebuilt_windows/lib/x64_debug/glew32.lib)
    set_target_properties(glew PROPERTIES IMPORTED_IMPLIB_RELEASE ${CMAKE_SOURCE_DIR}/prebuilt_windows/lib/x64_release/glew32.lib)

    set_target_properties(glfw PROPERTIES IMPORTED_IMPLIB_DEBUG ${CMAKE_SOURCE_DIR}/prebuilt_windows/lib/x64_debug/glfw3dll.lib)
    set_target_properties(glfw PROPERTIES IMPORTED_IMPLIB_RELEASE ${CMAKE_SOURCE_DIR}/prebuilt_windows/lib/x64_release/glfw3dll.lib)

    set_target_properties(jpeg PROPERTIES IMPORTED_IMPLIB_DEBUG ${CMAKE_SOURCE_DIR}/prebuilt_windows/lib/x64_debug/jpeg.lib)
    set_target_properties(jpeg PROPERTIES IMPORTED_IMPLIB_RELEASE ${CMAKE_SOURCE_DIR}/prebuilt_windows/lib/x64_release/jpeg.lib)
else()
    target_link_libraries(util
        -L/usr/lib/
        -L/usr/local/lib/
        -L/opt/local/lib/
        -lglfw
        -lglew
        -framework\ OpenGL
        -ljpeg
        physics
    )
endif()

FIND_PACKAGE(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

target_link_libraries(util
    glfw
    glew
    jpeg
    physics
    ${OPENGL_LIBRARIES}
)

install(TARGETS physics DESTINATION bin/)
install(TARGETS util DESTINATION bin/)

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

if (CMAKE_BUILD_TYPE MATCHES "debug")
    set(LIB_DIR "debug")
else (CMAKE_BUILD_TYPE MATCHES "debug")
    set(LIB_DIR "release")
endif ()

# Install Windows shared libraries
if ("${CMAKE_SYSTEM}" MATCHES "Windows")
    install(
        CODE "file(GLOB    _LIBS_ \"${CMAKE_SOURCE_DIR}/prebuilt_windows/lib/x64_${LIB_DIR}/*.dll\")"
        CODE "file(INSTALL \${_LIBS_} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)"
    )
endif()