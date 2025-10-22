#include <iostream>
#include <string>

//注意：glad头文件必须在glfw引用之前引用
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "wrapper/checkError.h"
#include "application/Application.h"
#include "glframework/shader.h"

#include "stb_image.h"

GLuint ebo, vao;
GLuint texture;
Shader* shader = nullptr;

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

    float uvs[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.5f, 1.0f
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

    GLuint uvVbo = 0;
    GL_CALL(glGenBuffers(1, &uvVbo));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, uvVbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW));

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

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, uvVbo));
    GL_CALL(glEnableVertexAttribArray(2));
    GL_CALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL));

    //5.2 添加EBO到当前VAO中
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));

    GL_CALL(glBindVertexArray(0));
}

void prepareShader() {
    shader = new Shader("assets/shaders/16_texture_sampler/vertex.glsl", "assets/shaders/16_texture_sampler/fragment.glsl");
}

void prepareTexture() {
    //1 stbImage 读取图片
    int width, height, channels;

    //反转y轴
    stbi_set_flip_vertically_on_load(1);

    unsigned char* data = stbi_load("assets/textures/goku.jpg", &width, &height, &channels, STBI_rgb_alpha);

    //2 生成纹理并且激活单元绑定
    glGenTextures(1, &texture);
    //--激活纹理对象--
    glActiveTexture(GL_TEXTURE0);
    //--绑定纹理对象--
    glBindTexture(GL_TEXTURE_2D, texture);

    //3 传输纹理数据，开辟显存
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    //4 释放数据
    stbi_image_free(data);

    //5 设置纹理的过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    //6 设置纹理的包裹方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //u
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //v

    //由于前面已经激活纹理单元，并且绑定了纹理，如果这里解绑将会导致纹理单元中无纹理
    //glBindTexture(GL_TEXTURE_2D, 0);
}

void render() {
    //如果render最后不解绑vao，这句代码会导致程序异常
    //GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    //执行opengl画布清理操作
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

    //1 绑定当前program
    shader->begin();

    shader->setInt("sampler", 0);

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

    prepareTexture();
    prepareShader();
    prepareVao();

    while (app->update()) {
        render();
    }

    // 释放资源
    delete shader;

    app->destroy();

    return 0;
}