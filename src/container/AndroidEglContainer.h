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
    RdrWorld* world;

    void SetAttrs(android_app* app, void (*cnsdrwFunc)(Container*,RdrWorld*), RdrWorld* world);

    virtual void Deinit() {
        world->nodes.clear();
        EglContainer::Deinit();
        animating = false;
    }
   
    static void AndroidHandleCmd(android_app* app, int32_t cmd);
    void (*ConstructDraw)(Container*,RdrWorld*);
    virtual void Main();
};
