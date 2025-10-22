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
    shader = new Shader("assets/shaders/21_glm/vertex.glsl", "assets/shaders/21_glm/fragment.glsl");
}

void prepareTexture() {
    texture = new Texture("assets/textures/goku.jpg", 0);
}

void render() {
    //执行opengl画布清理操作
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

    //绑定当前的program
    shader->begin();

    shader->setInt("sampler", 0);
     
    //绑定当前的vao
    GL_CALL(glBindVertexArray(vao));

    //发出绘制指令
    GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

    //解绑vao
    GL_CALL(glBindVertexArray(0));

    shader->end();
}

void testGLM() {
    //向量
    glm::vec2 v0(0);
    glm::vec3 v1(0);
    glm::vec4 v2(0);

    glm::vec4 vadd = v2 + glm::vec4(0);

    auto multi = vadd * v2; //对应元素相乘，放到对应位置
    auto dotRes = glm::dot(vadd, v2);

    glm::vec3 vt0, vt1;
    auto crossRes = glm::cross(vt0, vt1); //向量叉乘只支持三维

    //矩阵
    glm::mat4 m0(1.0);
    glm::mat4 m1 = glm::identity<glm::mat4>();
    glm::mat2 mm2(1.0);
    glm::mat3 mm3(1.0);
    glm::mat2x3 mm4(1.0);

    std::cout << glm::to_string(mm4) << std::endl;

    auto madd = m0 + m1;
    auto mmulti = m0 * m1;
    auto res = m0 * v2;

    //transpose转置矩阵
    auto transMat = glm::transpose(madd);

    //inverse逆矩阵
    auto inverseMat = glm::inverse(madd);
}

int main() {
    //测试glm库
    testGLM();

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

    while (app->update()) {
        render();
    }

    // 释放资源
    delete shader;
    delete texture;

    app->destroy();

    return 0;
}