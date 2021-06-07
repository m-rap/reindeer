cmake_minimum_required(VERSION 3.4.1)

project(reindeer)

set(GLM_SRC_DIR ../glm CACHE STRING "glm library source dir")
set(BULLET_SRC_DIR ../bullet3 CACHE STRING "bullet3 library source dir")
set(GLFW_SRC_DIR ../glfw CACHE STRING "glfw library source dir")
set(USE_GLFW false CACHE STRING "enable glfw")
set(USE_EGL false CACHE STRING "enable EGL")

add_subdirectory(${GLM_SRC_DIR} glm)

set(TMP ${CMAKE_CXX_FLAGS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(BUILD_BULLET3 ON)
set(BUILD_OPENGL3_DEMOS OFF)
set(BUILD_BULLET2_DEMOS OFF)
set(BUILD_EXTRAS OFF)
set(BUILD_UNIT_TESTS OFF)
set(BUILD_CPU_DEMOS OFF)
set(BUILD_ENET OFF)
set(BUILD_CLSOCKET OFF)
set(INSTALL_LIBS OFF)
set(INSTALL_CMAKE_FILES OFF)
add_subdirectory(${BULLET_SRC_DIR} bullet3)
set(CMAKE_CXX_FLAGS "${TMP}")

message("ANDROID_NDK ${ANDROID_NDK}")
message("CMAKE_SYSTEM_NAME ${CMAKE_SYSTEM_NAME}")
message("GLM_SRC_DIR ${GLM_SRC_DIR}")
message("BULLET_SRC_DIR ${BULLET_SRC_DIR}")
message("GLFW_SRC_DIR ${GLFW_SRC_DIR}")
message("USE_GLFW ${USE_GLFW}")
message("USE_EGL ${USE_EGL}")

set(USING_TERMUX false)
if (NOT ANDROID_NDK AND ${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    set(ANDROID_NDK /data/data/com.termux/files/home/android-ndk-r21d)
    set(USING_TERMUX TRUE)
endif()

set(EXAMPLE_BUILD_FLAGS "-Wno-writable-strings -DUSE_OPENGL")
set(BUILD_FLAGS "${EXAMPLE_BUILD_FLAGS} -fPIC")

set(REINDEER_SRC
    src/container/Container.cpp
    src/core/RdrTransform.cpp
    src/core/RdrHelper.cpp
    src/core/RdrNode.cpp
    src/core/RdrWorld.cpp
    src/core/RdrWorld_OpenGL.cpp
    src/core/RdrCollisionShapeFactory.cpp
    src/core/RdrCamera.cpp
    src/core/RdrDrawable.cpp
    src/core/RdrLight.cpp
    src/core/RdrLight_LegacyOpenGL.cpp
    src/core/RdrLight_OpenGL.cpp
    src/core/RdrMesh.cpp
    src/core/RdrMeshFactory_OpenGL.cpp
    src/core/RdrMesh_LegacyOpenGL.cpp
    src/core/RdrMesh_OpenGL.cpp
    src/core/RdrNode.cpp
    src/core/RdrRigidBody.cpp
    src/core/RdrShaderManager.cpp
    src/core/RdrTransform.cpp
    src/core/objloader.cpp
    src/core/shader.cpp
    src/core/config.cpp
    src/core/texture.cpp
)
set(REINDEER_INCLUDE
    src/core
    ${BULLET_SRC_DIR}/src
)

if (ANDROID_NDK)
    set(REINDEER_SRC
        ${REINDEER_SRC}
        src/container/AndroidEglContainer.cpp
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
    )
    set(REINDEER_INCLUDE
        ${REINDEER_INCLUDE}
        ${ANDROID_NDK}/sources/android/native_app_glue
    )
    set(EXAMPLE_BUILD_FLAGS "${BUILD_FLAGS} -DUSE_GLES")
    set(BUILD_FLAGS "${BUILD_FLAGS} -DUSE_GLES")
else()
    if (USE_GLFW)
        message("configuring glfw")
        add_subdirectory(${GLFW_SRC_DIR} glfw)
        set(REINDEER_SRC
            ${REINDEER_SRC}
            src/container/GlfwContainer.cpp
        )
        set(EXAMPLE_BUILD_FLAGS "${BUILD_FLAGS} -DUSE_GLFW")
        set(BUILD_FLAGS "${BUILD_FLAGS} -DUSE_GLFW")
    else()
        set(REINDEER_SRC
            ${REINDEER_SRC}
            src/container/GlxContainer.cpp
        )
    endif()
    if (USE_EGL)
        set(REINDEER_SRC
            ${REINDEER_SRC}
            src/container/EglContainer.cpp
        )
        set(EXAMPLE_BUILD_FLAGS "${BUILD_FLAGS} -DUSE_EGL")
        set(BUILD_FLAGS "${BUILD_FLAGS} -DUSE_EGL")
    endif()
endif()

add_library(reindeer
    STATIC
    ${REINDEER_SRC}
)

target_include_directories(reindeer PUBLIC
    ${REINDEER_INCLUDE}
)

#target_link_directories(reindeer PUBLIC
#    ${CMAKE_BINARY_DIR}/glm
#    ${CMAKE_BINARY_DIR}/bullet3/src/Bullet3Commom
#)

set(REINDEER_LIB
    glm
    Bullet3Common
    BulletDynamics
    BulletCollision
    LinearMath
)
if(USE_GLFW)
    set(REINDEER_LIB
        ${REINDEER_LIB}
        glfw
    )
endif()
target_link_libraries(reindeer
    ${REINDEER_LIB}
)

set_target_properties(reindeer PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS})

if (ANDROID_NDK)
    add_library(reindeer-example
        SHARED
        examples/fallingboxes-android.cpp
    )
    
    target_link_libraries(reindeer-example
        reindeer
        android
        EGL
        GLESv1_CM
        GLESv2
        log
    )
    
    set_target_properties(reindeer-example
        PROPERTIES COMPILE_FLAGS ${EXAMPLE_BUILD_FLAGS}
    )

    set(EXAMPLE_LINK_FLAGS "-u ANativeActivity_onCreate")

    message("USING_TERMUX ${USING_TERMUX}")
    if (USING_TERMUX)
        set(EXAMPLE_LINK_FLAGS "${EXAMPLE_LINK_FLAGS} -Wl,--no-undefined")
    endif()
    message("EXAMPLE_LINK_FLAGS ${EXAMPLE_LINK_FLAGS}")

    set_target_properties(reindeer-example
        PROPERTIES LINK_FLAGS ${EXAMPLE_LINK_FLAGS}
    )
else()
    find_package(GLEW 2 REQUIRED)
    
    add_executable(reindeer-example
        examples/fallingboxes.cpp
    )
    
    set_target_properties(reindeer-example
        PROPERTIES COMPILE_FLAGS ${EXAMPLE_BUILD_FLAGS}
    )
    
    set(EXAMPLE_LIB
        reindeer
        GLEW::GLEW
    )

    if (USE_EGL)
        set(EXAMPLE_LIB
            ${EXAMPLE_LIB}
            EGL
        )
    endif()

    if (APPLE)
        set(EXAMPLE_LIB
            ${EXAMPLE_LIB}
            "-framework OpenGL"
        )
    else()
        set(EXAMPLE_LIB
            ${EXAMPLE_LIB}
            GL
            X11
        )
    endif()

    target_link_libraries(reindeer-example
        ${EXAMPLE_LIB}
    )
    
    add_custom_target(copyResDir ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/res
    )
    add_custom_target(copyShaderDir ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shader ${CMAKE_BINARY_DIR}/shader
    )
endif()
