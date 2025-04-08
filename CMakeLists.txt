cmake_minimum_required(VERSION 3.15)
project(raylib_test C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
    set(ARCH_NAME "arm64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm|ARM")
    set(ARCH_NAME "arm")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCH_NAME "x64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(ARCH_NAME "x86")
else()
    set(ARCH_NAME "unknown")
endif()

if(PLATFORM STREQUAL "Wasm")
    set(PLATFORM_DIR "Wasm")
    set(RAYLIB_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/external/raylib-5.5/${PLATFORM_DIR}")
    set(RAYLIB_LIBRARY "${RAYLIB_PREFIX}/lib/libraylib.a")

    set(EMSCRIPTEN_FLAGS
        "-s USE_GLFW=3"
        "-s ASSERTIONS=1"
        "-s WASM=1"
        "-s ASYNCIFY"
        "-s EXPORTED_RUNTIME_METHODS=['ccall','cwrap']"
        "--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/bin/resources@resources"
        "--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/bin/shaders@shaders"
    )
    string(REPLACE ";" " " EMSCRIPTEN_FLAGS_STR "${EMSCRIPTEN_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${EMSCRIPTEN_FLAGS_STR}")

    set(PLATFORM_LIBS "")
elseif(APPLE)
    set(PLATFORM_DIR "macOS-universal")
    set(RAYLIB_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/external/raylib-5.5/${PLATFORM_DIR}")
    set(RAYLIB_LIBRARY "${RAYLIB_PREFIX}/lib/libraylib.a")

    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(OPENGL_LIBRARY OpenGL REQUIRED)

    set(PLATFORM_LIBS ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${OPENGL_LIBRARY})

elseif(WIN32)
    set(PLATFORM_DIR "Windows-x64")
    set(RAYLIB_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/external/raylib-5.5/${PLATFORM_DIR}")
    set(RAYLIB_LIBRARY "${RAYLIB_PREFIX}/lib/raylib.lib")

    set(PLATFORM_LIBS winmm opengl32)

elseif(UNIX AND NOT APPLE)
    set(PLATFORM_DIR "Linux-x64")
    set(RAYLIB_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/external/raylib-5.5/${PLATFORM_DIR}")
    set(RAYLIB_LIBRARY "${RAYLIB_PREFIX}/lib/libraylib.a")

    find_package(X11 REQUIRED)
    find_package(OpenGL REQUIRED)
    find_package(Threads REQUIRED)

    set(PLATFORM_LIBS ${X11_LIBRARIES} ${OPENGL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} dl m)
endif()

set(RAYLIB_INCLUDE_DIR "${RAYLIB_PREFIX}/include")

file(GLOB_RECURSE SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${RAYLIB_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/external
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${RAYLIB_LIBRARY}
    ${PLATFORM_LIBS}
)

if(PLATFORM STREQUAL "Wasm")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/wasm/public/assets"
    )
else()
    set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
    )
endif()

if(MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS "/NODEFAULTLIB:MSVCRT"
    )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/bin/resources
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/bin/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_SOURCE_DIR}/src/resources
    ${CMAKE_SOURCE_DIR}/bin/resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_SOURCE_DIR}/src/shaders
    ${CMAKE_SOURCE_DIR}/bin/shaders
)

if(PLATFORM STREQUAL "Wasm")
    add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/src/wasm/index.html
        ${CMAKE_SOURCE_DIR}/bin/wasm/public/index.html

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/src/wasm/serve.js
        ${CMAKE_SOURCE_DIR}/bin/wasm/serve.js
    )
endif()
