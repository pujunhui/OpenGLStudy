#include "trackBallCameraControl.h"


TrackBallCameraControl::TrackBallCameraControl() {

}

TrackBallCameraControl::~TrackBallCameraControl() {

}

void TrackBallCameraControl::onCursor(double xpos, double ypos) {
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
        mCamera->mPosition -= mCamera->mRight * deltaX;
    }

    mCurrentX = xpos;
    mCurrentY = ypos;
}

void TrackBallCameraControl::onScroll(double offset) {
    mCamera->scale(mScaleSpeed * offset);
}

void TrackBallCameraControl::update() {
    //最终移动方向
    glm::vec3 direction(0.0f);

    auto front = glm::cross(mCamera->mUp, mCamera->mRight);
    auto right = mCamera->mRight;

    if (mKeyMap[GLFW_KEY_W]) {
        direction += front;
    }
    if (mKeyMap[GLFW_KEY_S]) {
        direction -= front;
    }
    if (mKeyMap[GLFW_KEY_A]) {
        direction -= right;
    }
    if (mKeyMap[GLFW_KEY_D]) {
        direction += right;
    }

    //此时direction有可能不为1的长度，也可能是0的长度
    if (glm::length(direction) != 0) {
        direction = glm::normalize(direction);
        mCamera->mPosition += direction * mSpeed;
    }
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

    //影响当前相机的up/right向量和位置
    mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f);
    mCamera->mRight = mat * glm::vec4(mCamera->mRight, 0.0f);
    mCamera->mPosition = mat * glm::vec4(mCamera->mPosition, 1.0f);
}