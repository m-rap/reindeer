#include "GlfwContainer.h"

GlfwContainer::GlfwContainer()
{
    lastTimeMiddleMousePressed = glfwGetTime();
    mouseX = -1.0;
    mouseY = -1.0;
}

GlfwContainer::~GlfwContainer()
{
    if (window)
        glfwTerminate();
}

void GlfwContainer::ScrollCb(GLFWwindow* window, double xoffset, double yoffset)
{
    GlfwContainer* container = (GlfwContainer*)glfwGetWindowUserPointer(window);
    container->Scrolled(xoffset, yoffset);
}

void GlfwContainer::KeyCb(GLFWwindow *window, int key, int scancodes, int action, int mods)
{
    GlfwContainer* container = (GlfwContainer*)glfwGetWindowUserPointer(window);
    if (action == GLFW_PRESS)
        container->KeyPressed(key);
    if (action == GLFW_RELEASE)
        container->KeyReleased(key);
}

void GlfwContainer::ResizeCb(GLFWwindow *window, int width, int height)
{
    GlfwContainer* container = (GlfwContainer*)glfwGetWindowUserPointer(window);
    if (width != SCREEN_WIDTH || height != SCREEN_HEIGHT) {
        container->OnResize(width, height);
    }
}

void GlfwContainer::SubInit()
{
    int init = glfwInit();
    if (init == GL_FALSE) {
        printf("glfw init failed\n");
        return;
    }

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Reindeer", NULL, NULL);
    if (!window) {
        printf("glfw create window failed\n");
        glfwTerminate();
        return;
    }

    glfwSetWindowUserPointer(window, this);
    glfwMakeContextCurrent(window);

    glfwSetScrollCallback(window, ScrollCb);
    glfwSetKeyCallback(window, KeyCb);
    glfwSetWindowSizeCallback(window, ResizeCb);
}

void GlfwContainer::Deinit()
{
}

void GlfwContainer::PreUpdate()
{
}

void GlfwContainer::PostUpdate()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void GlfwContainer::ReadInput()
{
    double tempX = mouseX, tempY = mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    if (tempX != mouseX || tempY != mouseY) {
        MouseMoved(mouseX, mouseY);
    }

    int mouseRightButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
    if (mouseRightButtonState == GLFW_PRESS)
        MouseRightButtonPressed(mouseX, mouseY);
    else if (mouseRightButtonState == GLFW_RELEASE)
        MouseRightButtonReleased(mouseX, mouseY);

    int mouseMiddleButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
    if (mouseMiddleButtonState == GLFW_PRESS) {
        lastTimeMiddleMousePressed = glfwGetTime();
        MouseMiddleButtonPressed(mouseX, mouseY);
    }

    double tempMid = glfwGetTime() - lastTimeMiddleMousePressed;
    if (tempMid > 0.05 && mouseRightButtonState == GLFW_RELEASE)
        MouseMiddleButtonReleased(mouseX, mouseY);

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        KeyPressed(GLFW_KEY_LEFT);

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        KeyPressed(GLFW_KEY_RIGHT);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        KeyPressed(GLFW_KEY_UP);

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        KeyPressed(GLFW_KEY_DOWN);

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
        KeyPressed(GLFW_KEY_M);
}
