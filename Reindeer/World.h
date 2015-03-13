#ifndef WORLD_H
#define WORLD_H

#include "shader.h"
#include "Drawable.h"
#include "Camera.h"
#include "BoxObject.h"
#include "ModelObject.h"
#include "Light.h"

#include "Container.h"

using namespace std;

class World :
    public ContainerEventListener
{
protected:

	BoxObject* ground;

	bool mouseRightButtonDown, mouseMiddleButtonDown;
	unsigned long lastTimeMiddleMousePressed;
	double mouseX, mouseY;

	Container* container;

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
