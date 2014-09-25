#ifndef WORLD_H
#define WORLD_H

#include "shader.h"
#include "Drawable.h"
#include "Camera.h"
#include "BoxObject.h"

using namespace std;

#ifdef _MSC_VER
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // the WindowProc function prototype
#endif

class World
{
private:
    GLuint lightPositionId;
    float lightPosition[4];
#ifdef _MSC_VER
	HDC hDC;		// Private GDI Device Context
#else
    GLFWwindow* window;
#endif
public:
    World();
    virtual ~World();

    Camera camera;
    GLuint programId;

    vector<Drawable*> DrawableObjects;

    void Init();
    void Render();
};

#endif // WORLD_H
