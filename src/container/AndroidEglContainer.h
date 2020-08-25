#include "EglContainer.h"
#include "RdrWorld_OpenGL.h"

class AndroidEglContainer : public EglContainer {
protected:
    virtual void SubInit() {
        animating = false;
        window = app->window;
        EglContainer::SubInit();
    }
public:
    android_app* app;
    bool animating;
    RdrWorld_OpenGL* world;

    void SetAttrs(android_app* app, void (*cnsdrwFunc)(Container&,RdrWorld_OpenGL&), RdrWorld_OpenGL* world);

    virtual void Deinit() {
        world->nodes.clear();
        EglContainer::Deinit();
        animating = false;
    }
   
    static void AndroidHandleCmd(android_app* app, int32_t cmd);
    void (*ConstructDraw)(Container&,RdrWorld_OpenGL&);
    virtual void Main();
};
