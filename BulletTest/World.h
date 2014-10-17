#ifndef WORLD_H
#define WORLD_H

#include "shader.h"
#include "Drawable.h"
#include "Camera.h"
#include "BoxObject.h"
#include "ModelObject.h"

#ifdef _MSC_VER
#include <windows.h>
#include <windowsx.h>
#else
#include <GLFW/glfw3.h>
#endif

using namespace std;

#ifdef _MSC_VER
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // the WindowProc function prototype
#endif

class World
{
protected:
#ifdef _MSC_VER
	HWND hWnd;
	HDC hDC;		// Private GDI Device Context
#else
    GLFWwindow* window;
#endif
	void InitWindow();
	virtual void Init3d() = 0;
	virtual void PreUpdate() = 0;
	virtual void PostUpdate() = 0;
	virtual void PostRender() = 0;
	void Integrate(btDiscreteDynamicsWorld* dynamicsWorld, float dt);

public:
    World();
    virtual ~World();

    Camera camera;
    GLuint programId;

    vector<Drawable*> DrawableObjects;
	vector<PhysicalObject*> PhysicalObjects;
	
    void Init();
    void Render();
};

#endif // WORLD_H
