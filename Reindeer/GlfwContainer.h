#ifndef GLFWCONTAINER_H
#define GLFWCONTAINER_H

#include "Container.h"
#include "reindeer.h"
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

	static unordered_map<GLFWwindow*, GlfwContainer*> ContainerMap;

    virtual int ShouldClose() { return glfwWindowShouldClose(window); }
	virtual void Init();
    virtual void PreUpdate();
    virtual void PostUpdate();
    virtual void ReadInput();
};

#endif // GLFWCONTAINER_H
