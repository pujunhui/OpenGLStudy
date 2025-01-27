#pragma once 

#include "../../glframework/core.h"
#include "camera.h"
#include <map>

class CameraControl {
public:
    virtual void onMouse(int button, int action, double xpos, double ypos);
    virtual void onCurosr(double xpos, double ypos);
    virtual void onKey(int key, int action, int mods);
    virtual void onScroll(double offset);

    //每一帧渲染之前都要进行调用，每一更新的行为可以放在这里
    virtual void update();

    void setCamera(Camera* camera) {
        mCamera = camera;
    }

    void setSensitivity(float s) {
        mSensitivity = s;
    }

    void setScaleSpeed(float s) {
        mScaleSpeed = s;
    }

protected:
    //鼠标的按键状态
    bool mLeftMouseDown = false;
    bool mRightMouseDown = false;
    bool mMiddleMouseDown = false;
    //键盘按键状态
    std::map<int, bool> mKeyMap;

    //当前鼠标位置
    double mCurrentX = 0.0, mCurrentY = 0.0;
    //灵敏度
    float mSensitivity = 0.2f;
    Camera* mCamera = nullptr;

    //缩放速度
    float mScaleSpeed = 0.2f;
};