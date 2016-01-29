#include "Container.h"

Container::Container()
{
    loaded = false;
}

Container::~Container()
{
    listeners.clear();
}

void Container::AddListener(ContainerEventListener* listener)
{
    listeners.push_back(listener);
}

void Container::Init(int argc, char *argv[])
{
    this->argc = argc;
    this->argv = argv;
    SubInit();
    OnLoad();
}

void Container::Main()
{
    while (true) {
        ReadInput();
        if (ShouldClose())
            break;

        OnPaint();
    }

    OnClosing();
}

void Container::OnLoad()
{
    if (loaded)
        return;
    loaded = true;
    for (deque<ContainerEventListener*>::iterator it = listeners.begin(); it != listeners.end(); it++)
        (*it)->OnLoad(argc, argv);
}

void Container::OnPaint()
{
    for (deque<ContainerEventListener*>::iterator it = listeners.begin(); it != listeners.end(); it++)
        (*it)->OnPaint();
}

void Container::OnClosing()
{
    for (deque<ContainerEventListener*>::iterator it = listeners.begin(); it != listeners.end(); it++)
        (*it)->OnClosing();
}

void Container::Scrolled(double xoffset, double yoffset)
{
    for (deque<ContainerEventListener*>::iterator it = listeners.begin(); it != listeners.end(); it++)
        (*it)->Scrolled(xoffset, yoffset);
}

void Container::MouseLeftButtonPressed(double x, double y)
{
    for (deque<ContainerEventListener*>::iterator it = listeners.begin(); it != listeners.end(); it++)
        (*it)->MouseLeftButtonPressed(x, y);
}

void Container::MouseLeftButtonReleased(double x, double y)
{
    for (deque<ContainerEventListener*>::iterator it = listeners.begin(); it != listeners.end(); it++)
        (*it)->MouseLeftButtonReleased(x, y);
}

void Container::MouseRightButtonPressed(double x, double y)
{
    for (deque<ContainerEventListener*>::iterator it = listeners.begin(); it != listeners.end(); it++)
        (*it)->MouseRightButtonPressed(x, y);
}

void Container::MouseRightButtonReleased(double x, double y)
{
    for (deque<ContainerEventListener*>::iterator it = listeners.begin(); it != listeners.end(); it++)
        (*it)->MouseRightButtonReleased(x, y);
}

void Container::MouseMiddleButtonPressed(double x, double y)
{
    for (deque<ContainerEventListener*>::iterator it = listeners.begin(); it != listeners.end(); it++)
        (*it)->MouseMiddleButtonPressed(x, y);
}

void Container::MouseMiddleButtonReleased(double x, double y)
{
    for (deque<ContainerEventListener*>::iterator it = listeners.begin(); it != listeners.end(); it++)
        (*it)->MouseMiddleButtonReleased(x, y);
}

void Container::MouseMoved(double x, double y)
{
    for (deque<ContainerEventListener*>::iterator it = listeners.begin(); it != listeners.end(); it++)
        (*it)->MouseMoved(x, y);
}

void Container::KeyPressed(int keyCode)
{
    for (deque<ContainerEventListener*>::iterator it = listeners.begin(); it != listeners.end(); it++)
        (*it)->KeyPressed(keyCode);
}

void Container::KeyReleased(int keyCode)
{
    for (deque<ContainerEventListener*>::iterator it = listeners.begin(); it != listeners.end(); it++)
        (*it)->KeyReleased(keyCode);
}
