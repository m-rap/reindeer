#include "AndroidEglContainer.h"

void AndroidEglContainer::AndroidHandleCmd(android_app* app, int32_t cmd) {
    LOGI("engine_handle_cmd %d", cmd);
    if (app->userData == NULL) {
        return;
    }
    AndroidEglContainer* container1 = (AndroidEglContainer*)app->userData;
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            break;
        case APP_CMD_INIT_WINDOW:
            if (container1->app->window != NULL) {
                container1->Init(0, NULL);
                if (container1->ConstructDraw != NULL) {
                    container1->ConstructDraw(container1);
                }
            }
            break;
        case APP_CMD_TERM_WINDOW:
            container1->Deinit();
            break;
        case APP_CMD_GAINED_FOCUS:
            container1->animating = true;
            break;
        case APP_CMD_LOST_FOCUS:
            container1->animating = false;
            break;
        default:
            break;
    }
    LOGI("engine_handle_cmd %d done", cmd);
    
}

void AndroidEglContainer::SetAndroidAttr(android_app* _app, void (*cnsdrwFunc)(Container*)) {
    app = _app;
    ConstructDraw = cnsdrwFunc;
    app->userData = this;
    app->onAppCmd = AndroidEglContainer::AndroidHandleCmd;
}

void AndroidEglContainer::Main() {
    
    LOGI("android_main");

    int secDiff = 0, prevSecDiff = 0;
    timeval start;
    gettimeofday(&start, NULL);

    running = false;
    animating = false;

    android_app* state = app;

    int fps = 0;

    LOGI("to loop");

    while (true) {
        int ident;
        int events;
        android_poll_source* source;


        timeval now;
        gettimeofday(&now, NULL);
        secDiff = now.tv_sec - start.tv_sec;

        while ((ident = ALooper_pollAll(0, NULL, &events,
                                      (void**)&source)) >= 0) {

            if (source != NULL) {
                source->process(state, source);
            }

            if (ident == LOOPER_ID_USER) {
            }

            if (secDiff != prevSecDiff) {
            }

            if (state->destroyRequested != 0) {
                LOGI("destroyRequested");
                Deinit();
                break;
            }
        }

        if (state->destroyRequested != 0) {
            break;
        }

        if (secDiff != prevSecDiff) {
            //LOGI("fps %d", fps);
            fps = 0;
        }

        if (running && animating) {
            OnPaint();
        }

        fps++;

        prevSecDiff = secDiff;
    }

    LOGI("main end");
}
