#include "EglContainer.h"

class AndroidEglContainer : public EglContainer {
protected:
    virtual void SubInit() {
        EglContainer::SubInit();
        running = true;
    }
public:
    android_app* app;
    bool running;
    bool animating;

    void Init(android_app* app, void (*cnsdrwFunc)());
   
    static void AndroidHandleCmd(android_app* app, int32_t cmd);
    void (*ConstructDraw)(Container*);
    virtual void Main();
}
