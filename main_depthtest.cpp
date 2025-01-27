#include <iostream>

#include "glframework/core.h"
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

GLuint vao;
Shader* shader = nullptr;
Texture* texture = nullptr;

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

void OnMouse(int botton, int action, int mods) {
    double x, y;
    app->getCursorPosition(&x, &y);
    cameraControl->onMouse(botton, action, x, y);
    std::cout << "OnMouse(" << botton << ", " << action << ", " << x << ", " << y << ")" << std::endl;
}

void OnCursor(double xpos, double ypos) {
    cameraControl->onCurosr(xpos, ypos);
    std::cout << "OnCursor(" << xpos << ", " << ypos << ")" << std::endl;
}

void OnScroll(double offset) {
    cameraControl->onScroll(offset);
    std::cout << "OnScroll(" << offset << ")" << std::endl;
}

void prepareShader() {
    shader = new Shader("assets/shaders/vertex_glm.glsl", "assets/shaders/fragment_glm.glsl");
}

void prepareVAO() {
    //准备顶点数据数组
    float vertices[] = {
        //x    y   z       r     g    b      u    v
        -1.0, 1.0, 0.0,  1.0f, 0.0f, 0.0f,  0.0, 1.0,   //左上
        -1.0,-1.0, 0.0,  0.0f, 1.0f, 0.0f,  0.0, 0.0,   //左下
         1.0,-1.0, 0.0,  0.0f, 0.0f, 1.0f,  1.0, 0.0,   //右下
         1.0, 1.0, 0.0,  0.5f, 0.5f, 0.5f,  1.0, 1.0,   //右上
    };
    //准备顶点索引数组
    int indices[] = {
        0, 1, 2,
        2, 3, 0,
    };

    //vbo创建、绑定、填充数据
    GLuint vbo;
    GL_CALL(glGenBuffers(1, &vbo));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    //ebo创建、绑定、填充数据
    GLuint ebo;
    GL_CALL(glGenBuffers(1, &ebo));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    //创建vao
    GL_CALL(glGenVertexArrays(1, &vao));
    GL_CALL(glBindVertexArray(vao));

    //绑定vbo加入属性描述信息（前面已经绑定VBO了，这里即使不再次绑定，运行也是没问题的）
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));

    //加入位置属性描述信息
    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0));
    //加入颜色属性描述信息
    GL_CALL(glEnableVertexAttribArray(1));
    GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3)));
    //加入uv属性描述信息
    GL_CALL(glEnableVertexAttribArray(2));
    GL_CALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6)));

    //绑定ebo（注意：这里必须在绑定VAO之后再次绑定）
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));

    GL_CALL(glBindVertexArray(0));
}

void prepareTexture() {
    texture = new Texture("assets/textures/goku.jpg", 0);
}

void prepareCamera() {
    camera = new PerspectiveCamera(60.0f, (float)app->getWidth() / (float)app->getHeight(), 0.1f, 1000.0f);
    float size = 3.0f;
    //camera = new OrthographicCamera(-size, size, -size, size, size, -size); //看向的-z轴
    //cameraControl = new TrackBallCameraControl();
    cameraControl = new GameCameraControl();
    cameraControl->setCamera(camera);
    cameraControl->setSensitivity(0.8f);
}

void render() {
    //执行opengl画布清理操作
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    //绑定当前的program
    shader->begin();

    shader->setInt("sampler", 0);

    shader->setMatrix4x4("transform", glm::mat3(1.0f));
    shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
    shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

    //绑定当前的vao
    GL_CALL(glBindVertexArray(vao));

    //发出绘制指令
    GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

    glm::mat4 transform = glm::translate(glm::mat4(1.0), glm::vec3(1.0f, 0.0f, -0.1f));
    shader->setMatrix4x4("transform", transform);

    //发出绘制指令
    GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

    //解绑vao
    GL_CALL(glBindVertexArray(0));

    shader->end();
}

int main() {
    if (!app->init(800, 600)) {
        return -1;
    }

    app->setResizeCallback(OnResize);
    app->setKeyBoardCallback(OnKey);
    app->setMouseCallback(OnMouse);
    app->setCursorCallback(OnCursor);
    app->setScrollCallback(OnScroll);

    //设置opengl视口以及清理颜色
    GL_CALL(glViewport(0, 0, 800, 600));
    GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

    //开启深度缓存功能
    glEnable(GL_DEPTH_TEST);
    //设置深度测试方法
    glDepthFunc(GL_LESS);

    prepareShader();
    prepareVAO();
    prepareTexture();
    prepareCamera();

    while (app->update()) {
        cameraControl->update();
        render();
    }

    app->destroy();

    return 0;
}