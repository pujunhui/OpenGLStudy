#include "orthographicCamera.h"


OrthographicCamera::OrthographicCamera(float l, float r, float b, float t, float n, float f) {
    mLeft = l;
    mRight = r;
    mBottom = b;
    mTop = t;
    mNear = n;
    mFar = f;
}

OrthographicCamera::~OrthographicCamera() {

}

glm::mat4 OrthographicCamera::getProjectionMatrix() {
    float scale = std::pow(2.0f, mScale);
    return glm::ortho(mLeft * scale, mRight * scale, mBottom * scale, mTop * scale, mNear, mFar);
}

void OrthographicCamera::scale(float deltaScale) {
    mScale += deltaScale;
}
