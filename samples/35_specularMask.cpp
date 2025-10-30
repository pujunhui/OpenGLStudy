#include <iostream>

#include "glframework/core.h"
#include "glframework/shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "wrapper/checkError.h"
#include "application/Application.h"
#include "glframework/texture.h"

//引入camera+控制器
#include "application/camera/perspectiveCamera.h"
#include "application/camera/orthographicCamera.h"
#include "application/camera/gameCameraControl.h"
#include "application/camera/trackBallCameraControl.h"

#include "glframework/geometry.h"
#include "glframework/material/phongMaterial.h"
#include "glframework/mesh.h"
#include "glframework/renderer/renderer.h"
#include "util/helper.h"

Renderer* renderer = nullptr;
std::vector<Mesh*> meshes{};
DirectionalLight* dirLight = nullptr;
AmbientLight* ambLight = nullptr;

Camera* camera = nullptr;
CameraControl* cameraControl = nullptr;

void OnResize(int width, int height) {
    GL_CALL(glViewport(0, 0, width, height));
    std::cout << "OnResize(" << width << ", " << height << ")" << std::endl;
}

void OnKey(int key, int action, int mods) {
    cameraControl->onKey(key, action, mods);
    std::cout << "OnKey(" << key << ", " << action << ", " << mods << ")" << std::endl;
}

void OnMouse(int button, int action, int mods) {
    double x, y;
    app->getCursorPosition(&x, &y);
    cameraControl->onMouse(button, action, x, y);
    std::cout << "OnMouse(" << button << ", " << action << ", " << x << ", " << y << ")" << std::endl;
}

void OnCursor(double xpos, double ypos) {
    cameraControl->onCursor(xpos, ypos);
    std::cout << "OnCursor(" << xpos << ", " << ypos << ")" << std::endl;
}

void OnScroll(double offset) {
    cameraControl->onScroll(offset);
    std::cout << "OnScroll(" << offset << ")" << std::endl;
}

void prepare() {
    renderer = new Renderer();
    //1 创建geometry
    auto geometry = Geometry::createBox(1.0f);

    //2 创建一个material
    auto meterial = new PhongMaterial();
    meterial->mShiness = 32.0f;
    meterial->mDiffuse = new Texture("assets/textures/box.png", 0);
    meterial->mSpecularMask = new Texture("assets/textures/sp_mask.png", 1);

    //3 生成mesh
    auto mesh = new Mesh(geometry, meterial);

    meshes.push_back(mesh);
    
    dirLight = new DirectionalLight();
    ambLight = new AmbientLight();
    ambLight->mColor = glm::vec3(0.1f);
}

void prepareCamera() {
    camera = new PerspectiveCamera(60.0f, (float)app->getWidth() / (float)app->getHeight(), 0.1f, 1000.0f);
    float size = 3.0f;
    //camera = new OrthographicCamera(-size, size, -size, size, size, -size); //看向的-z轴
    cameraControl = new TrackBallCameraControl();
    //cameraControl = new GameCameraControl();
    cameraControl->setCamera(camera);
    cameraControl->setSensitivity(0.8f);
}

int main() {
    if (!app->init(1600, 1200)) {
        return -1;
    }

    app->setResizeCallback(OnResize);
    app->setKeyBoardCallback(OnKey);
    app->setMouseCallback(OnMouse);
    app->setCursorCallback(OnCursor);
    app->setScrollCallback(OnScroll);

    //设置opengl视口以及清理颜色
    GL_CALL(glViewport(0, 0, 1600, 1200));
    GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

    //开启深度缓存功能
    glEnable(GL_DEPTH_TEST);
    //设置深度测试方法
    glDepthFunc(GL_LESS);

    prepareCamera();
    prepare();

    while (app->update()) {
        printFps();
        cameraControl->update();
        renderer->render(meshes, camera, dirLight, ambLight);
    }

    // 释放资源
    delete camera;
    delete cameraControl;

    app->destroy();

    return 0;
}