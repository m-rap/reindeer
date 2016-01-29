#ifndef GLFWCONTAINER_H
#define GLFWCONTAINER_H

#include "Container.h"

#ifdef _MSC_VER
#define _GLFW_WIN32
#define _GLFW_WGL
#define _GLFW_USE_OPENGL
#endif
#include <GLFW/glfw3.h>

class GlfwContainer :
    public Container
{
protected:
    GLFWwindow* window;
    double lastTimeMiddleMousePressed;
    double mouseX, mouseY;

    virtual void SubInit();

public:
    GlfwContainer();
    virtual ~GlfwContainer();

    virtual int KEY_LEFT() { return GLFW_KEY_LEFT; }
    virtual int KEY_RIGHT() { return GLFW_KEY_RIGHT; }
    virtual int KEY_UP() { return GLFW_KEY_UP; }
    virtual int KEY_DOWN() { return GLFW_KEY_DOWN; }
    virtual int KEY_M() { return GLFW_KEY_M; }
    virtual int KEY_m() { return GLFW_KEY_M; }

    static unordered_map<GLFWwindow*, GlfwContainer*> ContainerMap;

    virtual int ShouldClose() { return glfwWindowShouldClose(window); }
    virtual void Deinit();
    virtual void PreUpdate();
    virtual void PostUpdate();
    virtual void ReadInput();
};

#endif // GLFWCONTAINER_H
