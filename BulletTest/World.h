#ifndef WORLD_H
#define WORLD_H

#include "shader.h"
#include "Drawable.h"
#include "Camera.h"
#include "BoxObject.h"

using namespace std;

class World
{
private:
    GLuint lightPositionId;
    float lightPosition[4];
    GLFWwindow* window;
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
