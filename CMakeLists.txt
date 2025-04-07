cmake_minimum_required(VERSION 3.15)
project(raylib_test C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Detect architecture
if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
    set(ARCH_NAME "arm64")
    add_compile_definitions(PLATFORM_ARCH="arm64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm|ARM")
    set(ARCH_NAME "arm")
    add_compile_definitions(PLATFORM_ARCH="arm")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCH_NAME "x64")
    add_compile_definitions(PLATFORM_ARCH="x64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(ARCH_NAME "x86")
    add_compile_definitions(PLATFORM_ARCH="x86")
else()
    set(ARCH_NAME "unknown")
    add_compile_definitions(PLATFORM_ARCH="unknown")
endif()

if(APPLE)
    set(PLATFORM_DIR "macOS-universal")
    set(RAYLIB_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/external/raylib-5.5/${PLATFORM_DIR}")
    set(RAYLIB_LIBRARY "${RAYLIB_PREFIX}/lib/libraylib.a")
    add_compile_definitions(PLATFORM_NAME="macOS")

    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(OPENGL_LIBRARY OpenGL REQUIRED)

    set(PLATFORM_LIBS ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${OPENGL_LIBRARY})

elseif(WIN32)
    set(PLATFORM_DIR "Windows-x64")
    set(RAYLIB_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/external/raylib-5.5/${PLATFORM_DIR}")
    set(RAYLIB_LIBRARY "${RAYLIB_PREFIX}/lib/raylib.lib")
    add_compile_definitions(PLATFORM_NAME="Windows")

    set(PLATFORM_LIBS winmm opengl32)

elseif(UNIX AND NOT APPLE)
    set(PLATFORM_DIR "Linux-x64")
    set(RAYLIB_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/external/raylib-5.5/${PLATFORM_DIR}")
    set(RAYLIB_LIBRARY "${RAYLIB_PREFIX}/lib/libraylib.a")
    add_compile_definitions(PLATFORM_NAME="Linux")

    find_package(X11 REQUIRED)
    find_package(OpenGL REQUIRED)
    find_package(Threads REQUIRED)

    set(PLATFORM_LIBS ${X11_LIBRARIES} ${OPENGL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} dl m)
endif()

set(RAYLIB_INCLUDE_DIR "${RAYLIB_PREFIX}/include")

file(GLOB_RECURSE SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
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

if(MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS "/NODEFAULTLIB:MSVCRT"
    )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/bin/resources
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/bin/shaders
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_SOURCE_DIR}/src/resources
    ${CMAKE_SOURCE_DIR}/bin/resources
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_SOURCE_DIR}/src/shaders
    ${CMAKE_SOURCE_DIR}/bin/shaders
)
