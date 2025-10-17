#include <iostream>
#include <string>

//注意：glad头文件必须在glfw引用之前引用
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "wrapper/checkError.h"
#include "application/Application.h"
#include "glframework/shader.h"

GLuint ebo, vao;
Shader *shader = nullptr;

void OnResize(int width, int height) {
    GL_CALL(glViewport(0, 0, width, height));
    std::cout << "OnResize" << std::endl;
}

void OnKey(int key, int action, int mods) {
    std::cout << key << std::endl;
}

void prepareVao() {
    //1 准备数据
    float positions[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
    };

    float colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
    };

    unsigned int indices[] = {
        0, 1, 2
    };

    //2 VBO创建
    GLuint posVbo = 0;
    GL_CALL(glGenBuffers(1, &posVbo));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posVbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));

    GLuint colorVbo = 0;
    GL_CALL(glGenBuffers(1, &colorVbo));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, colorVbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

    //3 EBO创建
    GL_CALL(glGenBuffers(1, &ebo));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    //4 生成vao并绑定
    GL_CALL(glGenVertexArrays(1, &vao));
    GL_CALL(glBindVertexArray(vao));

    //5 绑定VBO、EBO
    // 注意：VAO记录的状态包括：
    //      1、顶点属性指针（通过glVertexAttribPointer设置）
    //      2、启动的顶点属性（通过glEnableVertexAttribArray设置）
    //      3、绑定的EBO（当VAO绑定时调用glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo)）
    // 
    // 而glBindBuffer(GL_ARRAY_BUFFER, vbo)并不会让VAO记录VBO
    // 
    //5.1 添加属性的描述信息
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posVbo)); //只有绑定了posVbo，下面的属性描述才会与此vbo相关
    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, colorVbo));
    GL_CALL(glEnableVertexAttribArray(1));
    GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL));

    //5.2 添加EBO到当前VAO中
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));

    GL_CALL(glBindVertexArray(0));
}

void prepareShader() {
    shader = new Shader("assets/shaders/13_shaderClass/vertex.glsl", "assets/shaders/13_shaderClass/fragment.glsl");
}

void render() {
    //如果render最后不解绑vao，这句代码会导致程序异常
    //GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    //执行opengl画布清理操作
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

    //1 绑定当前program
    shader->begin();

    //2 绑定当前vao
    GL_CALL(glBindVertexArray(vao));

    //3 发出绘制指令
    GL_CALL(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0));

    //4 解绑当前vao，避免下一帧误操作vao
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
    prepareVao();

    while (app->update()) {
        render();
    }

    app->destroy();

    // 释放资源
    delete shader;

    return 0;
}
