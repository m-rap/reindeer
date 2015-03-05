#ifndef WORLD_H
#define WORLD_H

#include "shader.h"
#include "Drawable.h"
#include "Camera.h"
#include "BoxObject.h"
#include "ModelObject.h"
#include "Light.h"

#ifndef USE_OPENGL
#include <windows.h>
#include <windowsx.h>
#else
#ifdef _MSC_VER
#define _GLFW_WIN32
#define _GLFW_WGL
#define _GLFW_USE_OPENGL
#endif
#include <GLFW/glfw3.h>
#endif
#include "Container.h"

using namespace std;

#ifdef _MSC_VER
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // the WindowProc function prototype
#endif

class World :
    public ContainerEventListener
{
protected:

	BoxObject* ground;

	bool mouseRightButtonDown, mouseMiddleButtonDown;
	unsigned long lastTimeMiddleMousePressed;
	double mouseX, mouseY;

	Container* container;

#ifndef USE_OPENGL
	HWND hWnd;
	HDC hDC;		// Private GDI Device Context
#else
#endif
	void InitWindow();
	virtual void Init3d() = 0;
	virtual void PreRender() = 0;
	virtual void PreUpdate() = 0;
	virtual void PostUpdate() = 0;
	virtual void PostRender() = 0;
	virtual void Draw();

public:
    World(Container* container);
    virtual ~World();

	static World* Global;

    Camera camera;
	Light* light;

    vector<Drawable*> DrawableObjects;
	vector<PhysicalObject*> PhysicalObjects;

    void Init();
    void Render();

    virtual void Scrolled(double xoffset, double yoffset);
    virtual void MouseLeftButtonPressed  (double x, double y);
    virtual void MouseLeftButtonReleased (double x, double y);
    virtual void MouseRightButtonPressed (double x, double y);
    virtual void MouseRightButtonReleased(double x, double y);
    virtual void MouseMiddleButtonPressed (double x, double y);
    virtual void MouseMiddleButtonReleased(double x, double y);
    virtual void KeyPressed(int keyCode);
    virtual void KeyReleased(int keyCode);
    virtual void MouseMoved(double x, double y);
};

#endif // WORLD_H
