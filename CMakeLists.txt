cmake_minimum_required(VERSION 3.4.1)

project(reindeer)

add_subdirectory(../glm glm)

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
add_subdirectory(../bullet3 bullet3)
set(CMAKE_CXX_FLAGS "${TMP}")


message(INFO " varsss ANDROID_NDK ${ANDROID_NDK} CMAKE_SYSTEM_NAME ${CMAKE_SYSTEM_NAME}")

if (NOT ANDROID_NDK AND ${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    set(ANDROID_NDK /data/data/com.termux/files/home/android-ndk-r21d)
    set(USING_TERMUX TRUE)
endif()

set(EXAMPLE_BUILD_FLAGS "-Wno-writable-strings -DUSE_OPENGL")
set(BUILD_FLAGS "${EXAMPLE_BUILD_FLAGS} -fPIC")

set(REINDEER_SRC
    src/container/Container.cpp
    src/container/EglContainer.cpp
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
    ../bullet3/src
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
    set(REINDEER_SRC
        ${REINDEER_SRC}
        src/container/GlxContainer.cpp
        #src/container/GlfwContainer.cpp
    )
    #set(EXAMPLE_BUILD_FLAGS "${BUILD_FLAGS} -DUSE_GLFW")
    #set(BUILD_FLAGS "${BUILD_FLAGS} -DUSE_GLFW")
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

target_link_libraries(reindeer
    glm
    Bullet3Common
    BulletDynamics
    BulletCollision
    LinearMath
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

    message("USING_TERMUXXXX ${USING_TERMUX}")
    if (USING_TERMUX)
        set(EXAMPLE_LINK_FLAGS "${EXAMPLE_LINK_FLAGS} -Wl,--no-undefined")
    endif()

    set_target_properties(reindeer-example
        PROPERTIES LINK_FLAGS ${EXAMPLE_LINK_FLAGS}
    )
else()
    
    add_executable(reindeer-example
        examples/fallingboxes.cpp
    )
    
    set_target_properties(reindeer-example
        PROPERTIES COMPILE_FLAGS ${EXAMPLE_BUILD_FLAGS}
    )
    
    target_link_libraries(reindeer-example
        reindeer
        EGL
        GL
        X11
        GLEW
        #glfw
    )
endif()
