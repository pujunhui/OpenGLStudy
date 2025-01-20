#pragma once 

#include "../../glframework/core.h"
#include "camera.h"
#include <map>

class CameraControl {
public:
    virtual void onMouse(int button, int action, double xpos, double ypos);
    virtual void onCurosr(double xpos, double ypos);
    virtual void onKey(int key, int action, int mods);

    virtual void update();

    void setCamera(Camera* camera) {
        mCamera = camera;
    }

    void setSensitivity(float s) {
        mSensitivity = s;
    }

protected:
    bool mLeftMouseDown = false;
    bool mRightMouseDown = false;
    bool mMiddleMouseDown = false;
    std::map<int, bool> mKeyMap;

    double mCurrentX = 0.0, mCurrentY = 0.0;
    float mSensitivity = 0.02f;
    Camera* mCamera;
};