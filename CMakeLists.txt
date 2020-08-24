cmake_minimum_required(VERSION 3.4.1)

project(reindeer)

add_subdirectory(../glm glm)

set(TMP ${CMAKE_CXX_FLAGS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(BUILD_BULLET3 true)
set(BUILD_OPENGL3_DEMOS false)
set(BUILD_BULLET2_DEMOS false)
set(BUILD_EXTRAS false)
set(BUILD_UNIT_TESTS false)
add_subdirectory(../bullet3 bullet3)
set(CMAKE_CXX_FLAGS "${TMP}")


message(INFO " varsss ANDROID_NDK ${ANDROID_NDK} CMAKE_SYSTEM_NAME ${CMAKE_SYSTEM_NAME}")

if (NOT ANDROID_NDK AND ${CMAKE_SYSTEM_NAME} STREQUAL "Android")
set(ANDROID_NDK /data/data/com.termux/files/home/android-ndk-r21d)
endif()

set(BUILD_FLAGS "-Wno-writable-strings -DUSE_OPENGL -fPIC")

add_library(reindeer
    STATIC
    ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
    src/container/Container.cpp
    src/container/EglContainer.cpp
    src/container/AndroidEglContainer.cpp
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

target_include_directories(reindeer PUBLIC
    ${ANDROID_NDK}/sources/android/native_app_glue
    src/core
    ../bullet3/src
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
    PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS}
)
set_target_properties(reindeer-example
    PROPERTIES LINK_FLAGS -Wl,--no-undefined
)
