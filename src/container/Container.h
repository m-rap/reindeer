#ifndef CONTAINER_H
#define CONTAINER_H

#include <deque>
#include "../core/reindeer.h"

using namespace std;

class ContainerEventListener
{
public:
    virtual void OnLoad(int argc, char **argv) = 0;
    virtual void OnPaint() = 0;
    virtual void OnClosing() = 0;
    virtual void OnResize(int width, int height) = 0;
    virtual void Scrolled(double xoffset, double yoffset) = 0;
    virtual void MouseLeftButtonPressed  (double x, double y) = 0;
    virtual void MouseLeftButtonReleased (double x, double y) = 0;
    virtual void MouseRightButtonPressed (double x, double y) = 0;
    virtual void MouseRightButtonReleased(double x, double y) = 0;
    virtual void MouseMiddleButtonPressed (double x, double y) = 0;
    virtual void MouseMiddleButtonReleased(double x, double y) = 0;
    virtual void MouseMoved(double x, double y) = 0;
    virtual void KeyPressed(int keyCode) = 0;
    virtual void KeyReleased(int keyCode) = 0;
};

class Container
{
protected:
    deque<ContainerEventListener*> listeners;
    int argc;
    char** argv;
    bool loaded;

    virtual void SubInit() = 0;

public:
    Container();
    virtual ~Container();

    virtual int KEY_LEFT() = 0;
    virtual int KEY_RIGHT() = 0;
    virtual int KEY_UP() = 0;
    virtual int KEY_DOWN() = 0;
    virtual int KEY_M() = 0;
    virtual int KEY_m() = 0;

    void AddListener(ContainerEventListener* listener);

    virtual int ShouldClose() = 0;
    void Init(int argc, char *argv[]);
    virtual void Deinit() = 0;
    virtual void PreUpdate() = 0;
    virtual void PostUpdate() = 0;
    virtual void ReadInput() = 0;
    virtual void Main();
    virtual void OnLoad();

    void OnPaint();
    void OnClosing();
    void OnResize(int width, int height);
    void Scrolled(double xoffset, double yoffset);
    void MouseLeftButtonPressed(double x, double y);
    void MouseLeftButtonReleased(double x, double y);
    void MouseRightButtonPressed(double x, double y);
    void MouseRightButtonReleased(double x, double y);
    void MouseMiddleButtonPressed(double x, double y);
    void MouseMiddleButtonReleased(double x, double y);
    void MouseMoved(double x, double y);
    void KeyPressed(int keyCode);
    void KeyReleased(int keyCode);
};

#endif // CONTAINER_H
