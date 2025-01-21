#include "trackBallCameraControl.h"


TrackBallCameraControl::TrackBallCameraControl() {

}

TrackBallCameraControl::~TrackBallCameraControl() {

}

void TrackBallCameraControl::onCurosr(double xpos, double ypos) {
    if (mLeftMouseDown) {
        //调整相机的各类参数
        //1 计算经线和纬线旋转的增量角度(正负都有可能)
        float deltaX = (xpos - mCurrentX) * mSensitivity;
        float deltaY = (ypos - mCurrentY) * mSensitivity;

        //2 分开计算pitch和yaw
        pitch(-deltaY);
        yaw(-deltaX);
    }
    else if (mMiddleMouseDown) {
        float deltaX = (xpos - mCurrentX) * mMoveSpeed;
        float deltaY = (ypos - mCurrentY) * mMoveSpeed;

        mCamera->mPosition += mCamera->mUp * deltaY;
        mCamera->mPosition += -mCamera->mRight * deltaX;
    }

    mCurrentX = xpos;
    mCurrentY = ypos;
}

void TrackBallCameraControl::onScroll(double offset) {
    mCamera->scale(mScaleSpeed * offset);
}

void TrackBallCameraControl::pitch(float angle) {
    //绕着mRight向量旋转
    auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), mCamera->mRight);

    //影响当前相机的up向量和位置
    //将3维坐标补成4维齐次坐标，如果是一个向量补0,如果是一个点补1
    mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f);
    mCamera->mPosition = mat * glm::vec4(mCamera->mPosition, 1.0f);
}

void TrackBallCameraControl::yaw(float angle) {
    //绕着世界坐标系的y轴旋转
    auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

    //影响当前相机的right向量和位置
    mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f);
    mCamera->mRight = mat * glm::vec4(mCamera->mRight, 0.0f);
    mCamera->mPosition = mat * glm::vec4(mCamera->mPosition, 1.0f);
}