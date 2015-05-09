add_library(fbx      SHARED IMPORTED)
add_library(freetype SHARED IMPORTED)
add_library(glew     SHARED IMPORTED)
add_library(glfw     SHARED IMPORTED)
add_library(jpeg     SHARED IMPORTED)
add_library(png      SHARED IMPORTED)
add_library(zlib     SHARED IMPORTED)
add_library(json     SHARED IMPORTED)

if ("${CMAKE_SYSTEM}" MATCHES "Windows")
	include_directories(${CMAKE_SOURCE_DIR}/prebuilt/windows/x64/include)

    set_target_properties(fbx  PROPERTIES IMPORTED_IMPLIB_DEBUG          ${CMAKE_SOURCE_DIR}/prebuilt/windows/x64/lib/Debug/libfbxsdk.lib)
    set_target_properties(fbx  PROPERTIES IMPORTED_IMPLIB_RELEASE        ${CMAKE_SOURCE_DIR}/prebuilt/windows/x64/lib/Release/libfbxsdk.lib)
    set_target_properties(fbx  PROPERTIES IMPORTED_IMPLIB_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/prebuilt/windows/x64/lib/Release/libfbxsdk.lib)

    set_target_properties(glew PROPERTIES IMPORTED_IMPLIB_DEBUG          ${CMAKE_SOURCE_DIR}/prebuilt/windows/x64/lib/Debug/glew32d.lib)
    set_target_properties(glew PROPERTIES IMPORTED_IMPLIB_RELEASE        ${CMAKE_SOURCE_DIR}/prebuilt/windows/x64/lib/Release/glew32.lib)
    set_target_properties(glew PROPERTIES IMPORTED_IMPLIB_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/prebuilt/windows/x64/lib/Release/glew32.lib)

    set_target_properties(glfw PROPERTIES IMPORTED_IMPLIB_DEBUG          ${CMAKE_SOURCE_DIR}/prebuilt/windows/x64/lib/Debug/glfw3dll.lib)
    set_target_properties(glfw PROPERTIES IMPORTED_IMPLIB_RELEASE        ${CMAKE_SOURCE_DIR}/prebuilt/windows/x64/lib/Release/glfw3dll.lib)
    set_target_properties(glfw PROPERTIES IMPORTED_IMPLIB_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/prebuilt/windows/x64/lib/Release/glfw3dll.lib)

    set_target_properties(jpeg PROPERTIES IMPORTED_IMPLIB_DEBUG          ${CMAKE_SOURCE_DIR}/prebuilt/windows/x64/lib/Debug/jpeg.lib)
    set_target_properties(jpeg PROPERTIES IMPORTED_IMPLIB_RELEASE        ${CMAKE_SOURCE_DIR}/prebuilt/windows/x64/lib/Release/jpeg.lib)
    set_target_properties(jpeg PROPERTIES IMPORTED_IMPLIB_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/prebuilt/windows/x64/lib/Release/jpeg.lib)

    set_target_properties(png  PROPERTIES IMPORTED_IMPLIB_DEBUG          ${CMAKE_SOURCE_DIR}/prebuilt/windows/x64/lib/Debug/libpng16.lib)
    set_target_properties(png  PROPERTIES IMPORTED_IMPLIB_RELEASE        ${CMAKE_SOURCE_DIR}/prebuilt/windows/x64/lib/Release/libpng16.lib)
    set_target_properties(png  PROPERTIES IMPORTED_IMPLIB_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/prebuilt/windows/x64/lib/Release/libpng16.lib)

    set_target_properties(zlib PROPERTIES IMPORTED_IMPLIB_DEBUG          ${CMAKE_SOURCE_DIR}/prebuilt/windows/x64/lib/Debug/zlib.lib)
    set_target_properties(zlib PROPERTIES IMPORTED_IMPLIB_RELEASE        ${CMAKE_SOURCE_DIR}/prebuilt/windows/x64/lib/Release/zlib.lib)
    set_target_properties(zlib PROPERTIES IMPORTED_IMPLIB_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/prebuilt/windows/x64/lib/Release/zlib.lib)

    set_target_properties(json PROPERTIES IMPORTED_IMPLIB_DEBUG          ${CMAKE_SOURCE_DIR}/prebuilt/windows/x64/lib/Debug/lib_json.lib)
    set_target_properties(json PROPERTIES IMPORTED_IMPLIB_RELEASE        ${CMAKE_SOURCE_DIR}/prebuilt/windows/x64/lib/Release/lib_json.lib)
    set_target_properties(json PROPERTIES IMPORTED_IMPLIB_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/prebuilt/windows/x64/lib/Release/lib_json.lib)
else()
# TODO
endif()

if (CMAKE_BUILD_TYPE MATCHES "debug")
    set(BIN_DIR "Debug")
else()
    set(BIN_DIR "Release")
endif()

# Install Windows shared libraries
if ("${CMAKE_SYSTEM}" MATCHES "Windows")
    install(
        CODE "file(GLOB    _DLLS_ \"${CMAKE_SOURCE_DIR}/prebuilt/windows/x64/bin/${BIN_DIR}/*.dll\")"
        CODE "file(INSTALL \${_DLLS_} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)"
    )
endif()
