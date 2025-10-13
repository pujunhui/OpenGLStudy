#include <iostream>

//注意：glad头文件必须在glfw引用之前引用
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "wrapper/checkError.h"
#include "application/Application.h"

void OnResize(int width, int height) {
    GL_CALL(glViewport(0, 0, width, height));
    std::cout << "OnResize" << std::endl;
}

void OnKey(int key, int action, int mods) {
    std::cout << key << std::endl;
}

void prepareSingleBuffer() {
    //1 准备顶点位置数据与颜色数据
    float positions[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };
    float colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    //2 为位置&颜色数据各自生成一个vbo
    GLuint posVbo = 0, colorVbo = 0;
    GL_CALL(glGenBuffers(1, &posVbo));
    GL_CALL(glGenBuffers(1, &colorVbo));

    //3 给两个分开的vbo各自填充数据
    //positions填充数据
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posVbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));

    //colors填充数据
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, colorVbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW));

    //4 生成vao并绑定
    GLuint vao = 0;
    GL_CALL(glGenVertexArrays(1, &vao));
    GL_CALL(glBindVertexArray(vao));
    
    //5 分别将位置/颜色属性的描述信息加入vao当中
    //5.1描述位置属性
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posVbo)); //只有绑定了posVbo，下面的属性描述才会与此vbo相关
    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL));

    //5.2描述颜色属性
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, colorVbo));
    GL_CALL(glEnableVertexAttribArray(1));
    GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL));

    GL_CALL(glBindVertexArray(0));
}

void prepareInterleavedBuffer() {
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    //1 生成一个vbo
    GLuint vbo = 0;
    GL_CALL(glGenBuffers(1, &vbo));

    //2 绑定当前vbo，到opengl状态机的当前vbo插槽上
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));

    //3 向当前vbo传输数据，也是在开辟显存
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    //4 生成vao并绑定
    GLuint vao = 0;
    GL_CALL(glGenVertexArrays(1, &vao));
    GL_CALL(glBindVertexArray(vao));

    //5 分别将位置/颜色属性的描述信息加入vao当中
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo)); //只有绑定了posVbo，下面的属性描述才会与此vbo相关

    //5.1描述位置属性
    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));
    //5.2描述颜色属性
    GL_CALL(glEnableVertexAttribArray(1));
    GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))));

    GL_CALL(glBindVertexArray(0));
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

    prepareSingleBuffer();
    prepareInterleavedBuffer();

    while (app->update()) {
        //执行opengl画布清理操作
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
    }

    app->destroy();

    return 0;
}