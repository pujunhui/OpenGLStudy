#include "gameCameraControl.h"


GameCameraControl::GameCameraControl() {

}

GameCameraControl::~GameCameraControl() {

}

void GameCameraControl::onCurosr(double xpos, double ypos) {
    float deltaX = (xpos - mCurrentX) * mSensitivity;
    float deltaY = (ypos - mCurrentY) * mSensitivity;

    if (mRightMouseDown) {
        pitch(-deltaY);
        yaw(-deltaX);
    }

    mCurrentX = xpos;
    mCurrentY = ypos;
}

void GameCameraControl::onScroll(double offset) {
    mCamera->scale(mScaleSpeed * offset);
}

void GameCameraControl::update() {
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

void GameCameraControl::pitch(float angle) {
    mPitch += angle;
    if (mPitch > 89.0f || mPitch < -89.0f) {
        mPitch -= angle;
        return;
    }

    //在gameCameraControl的情况下，pitch不会影响到mPosition
    auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), mCamera->mRight);
    mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f);
}

void GameCameraControl::yaw(float angle) {
    //绕着世界坐标系的y轴旋转
    auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

    //影响当前相机的up/right向量
    mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f);
    mCamera->mRight = mat * glm::vec4(mCamera->mRight, 0.0f);
}
