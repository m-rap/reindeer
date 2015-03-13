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
public:
    GlfwContainer();
    virtual ~GlfwContainer();

	virtual int KEY_LEFT() ;
	virtual int KEY_RIGHT();
	virtual int KEY_UP()   ;
	virtual int KEY_DOWN() ;

	static unordered_map<GLFWwindow*, GlfwContainer*> ContainerMap;

    virtual int ShouldClose() { return glfwWindowShouldClose(window); }
	virtual void Init();
    virtual void PreUpdate();
    virtual void PostUpdate();
    virtual void ReadInput();
};

#endif // GLFWCONTAINER_H
