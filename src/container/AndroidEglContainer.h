#include "EglContainer.h"

class AndroidEglContainer : public EglContainer {
protected:
    virtual void SubInit() {
        animating = false;
        EglContainer::SubInit();
    }
public:
    android_app* app;
    bool animating;

    void Init(android_app* app, void (*cnsdrwFunc)(Container*));

    virtual void Deinit() {
       EglContainer::Deinit();
       animating = false;
    }
   
    static void AndroidHandleCmd(android_app* app, int32_t cmd);
    void (*ConstructDraw)(Container*);
    virtual void Main();
};
