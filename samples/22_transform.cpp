#include <iostream>

#include "glframework/core.h"
#include "glframework/shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "wrapper/checkError.h"
#include "application/Application.h"
#include "glframework/texture.h"

GLuint vao;
Shader* shader = nullptr;
Texture* texture = nullptr;

void OnResize(int width, int height) {
    GL_CALL(glViewport(0, 0, width, height));
    std::cout << "OnResize" << std::endl;
}

void OnKey(int key, int action, int mods) {
    std::cout << key << std::endl;
}

void prepareVAO() {
    //准备顶点数据数组
    float vertices[] = {
        //x    y   z       r     g    b      u    v
        -0.5, 0.5, -0.9,  1.0f, 0.0f, 0.0f,  0.0, 1.0,   //左上
        -0.5,-0.5, -0.9,  0.0f, 1.0f, 0.0f,  0.0, 0.0,   //左下
         0.5,-0.5, -0.9,  0.0f, 0.0f, 1.0f,  1.0, 0.0,   //右下
         0.5, 0.5, -0.9,  0.5f, 0.5f, 0.5f,  1.0, 1.0,   //右上
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

void prepareShader() {
    shader = new Shader("assets/shaders/22_transform/vertex.glsl", "assets/shaders/22_transform/fragment.glsl");
}

void prepareTexture() {
    texture = new Texture("assets/textures/goku.jpg", 0);
}

glm::mat4 transform(1.0f);

//旋转变换
void doRotationTransform() {
    //构建一个旋转矩阵，绕着z轴旋转45度角
    //rotate函数:用于生成旋转矩阵
    //bug1:rotate必须得到一个float类型的角度，c++的template
    //bug2:rotate函数接受的不是角度(degree)，接收的弧度(radians)
    //注意点:radians函数也是模板函数，切记要传入float类型数据，加f后缀
    transform = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
}

//平移变换
void doTranslationTransform() {
    transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));
}

//缩放变换
void doScaleTransform() {
    transform = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 1.0f));
}

//矩阵复合变换
void doTransform() {
    glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));
    //先旋转，再平移
    ///transform = translateMat * rotateMat;

    //先平移，再旋转
    transform = rotateMat * translateMat;
}

float degree = 0.0f;
float offset = 0.0f;

//矩阵叠加变换实验
void updateTransform() {
    //更新旋转角度和平移
    degree += 0.01f;
    offset += 0.001f;

    //目标一：旋转的矩形
    //transform = glm::rotate(glm::mat4(1.0f), glm::radians(degree), glm::vec3(0.0f, 0.0f, 1.0f));

    //目标二：先平移再叠加旋转
    //transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));
    //transform = glm::rotate(transform, glm::radians(degree), glm::vec3(0.0f, 0.0f, 1.0f));

    //目标三：先旋转再叠加平移
    //transform = glm::rotate(glm::mat4(1.0f), glm::radians(degree), glm::vec3(0.0f, 0.0f, 1.0f));
    //transform = glm::translate(transform, glm::vec3(0.5f, 0.0f, 0.0f));

    //目标四：先做一次缩放，再叠加平移
    transform = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 1.0f));
    transform = glm::translate(transform, glm::vec3(offset, 0.0f, 0.0f));
}

void render() {
    //执行opengl画布清理操作
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

    //绑定当前的program
    shader->begin();

    shader->setInt("sampler", 0);

    shader->setMatrix4x4("transform", transform);
     
    //绑定当前的vao
    GL_CALL(glBindVertexArray(vao));

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

    //设置opengl视口以及清理颜色
    GL_CALL(glViewport(0, 0, 800, 600));
    GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

    prepareShader();
    prepareVAO();
    prepareTexture();

    //doRotationTransform();
    //doTranslationTransform();
    //doScaleTransform();{
    //doTransform();

    while (app->update()) {
        updateTransform();
        render();
    }

    app->destroy();

    // 释放资源
    delete shader;
    delete texture;

    return 0;
}