#include "Container.h"

Container::Container()
{
}

Container::~Container()
{
    listeners.clear();
}

void Container::AddListener(ContainerEventListener* listener)
{
    listeners.push_back(listener);
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
