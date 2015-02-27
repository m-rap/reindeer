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

using namespace std;

#ifdef _MSC_VER
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // the WindowProc function prototype
#endif

class World
{
protected:
#ifndef USE_OPENGL
	HWND hWnd;
	HDC hDC;		// Private GDI Device Context
#else
    GLFWwindow* window;
#endif
	void InitWindow();
	virtual void Init3d() = 0;
	virtual void PreRender() = 0;
	virtual void PreUpdate() = 0;
	virtual void PostUpdate() = 0;
	virtual void PostRender() = 0;
	virtual void Draw();

	BoxObject* ground;

public:
    World();
    virtual ~World();

	static World* Global;

    Camera camera;
	Light* light;

    vector<Drawable*> DrawableObjects;
	vector<PhysicalObject*> PhysicalObjects;

    void Init();
    void Render();
};

#endif // WORLD_H
