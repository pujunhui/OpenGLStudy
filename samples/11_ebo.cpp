#include <iostream>
#include <string>

//注意：glad头文件必须在glfw引用之前引用
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "wrapper/checkError.h"
#include "application/Application.h"

GLuint ebo, vao, program;

void OnResize(int width, int height) {
    GL_CALL(glViewport(0, 0, width, height));
    std::cout << "OnResize" << std::endl;
}

void OnKey(int key, int action, int mods) {
    std::cout << key << std::endl;
}

void prepareVaoForTriangles() {
    //1 准备数据
    float positions[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 1, 3
    };

    //2 VBO创建
    GLuint posVbo = 0;
    GL_CALL(glGenBuffers(1, &posVbo));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posVbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));
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

    //5.2 添加EBO到当前VAO中
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));

    GL_CALL(glBindVertexArray(0));
}

void prepareShader() {
    //1 完成vs与fs的源代码，并且装入字符串
    const char* vertexShaderSource = 
        "#version 460 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
        "}\0";
    const char* fragmentShaderSource = 
        "#version 460 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";

    //2 创建Shader程序（vs、fs）
    GLuint vertex, fragment;
    vertex = GL_CALL(glCreateShader(GL_VERTEX_SHADER));
    fragment = GL_CALL(glCreateShader(GL_FRAGMENT_SHADER));

    //3 为shader程序输入shader代码
    GL_CALL(glShaderSource(vertex, 1, &vertexShaderSource, NULL));
    GL_CALL(glShaderSource(fragment, 1, &fragmentShaderSource, NULL));
    
    int success = 0;
    char infoLog[1024];
    //4 执行shader代码编译
    //4.1 编译vertex shader，并检查编译结果
    GL_CALL(glCompileShader(vertex));
    GL_CALL(glGetShaderiv(vertex, GL_COMPILE_STATUS, &success));
    if (!success) {
        GL_CALL(glGetShaderInfoLog(vertex, 1024, NULL, infoLog));
        std::cout << "Error: SHADER COMPILE ERROR " << "\n" << infoLog << std::endl;
    }
    //4.2 编译fragment shader，并检查编译结果
    GL_CALL(glCompileShader(fragment));
    GL_CALL(glGetShaderiv(fragment, GL_COMPILE_STATUS, &success));
    if (!success) {
        GL_CALL(glGetShaderInfoLog(fragment, 1024, NULL, infoLog));
        std::cout << "Error: SHADER COMPILE ERROR " << "\n" << infoLog << std::endl;
    }
    
    //5 创建一个Program壳子
    program = GL_CALL(glCreateProgram());

    //6 将vs与fs编译好的结果放到program这个壳子里
    GL_CALL(glAttachShader(program, vertex));
    GL_CALL(glAttachShader(program, fragment));

    //7 执行program的链接操作，形成最终可执行shader程序
    GL_CALL(glLinkProgram(program));
    //7.1 检查链接错误
    GL_CALL(glGetProgramiv(program, GL_LINK_STATUS, &success));
    if (!success) {
        GL_CALL(glGetProgramInfoLog(program, 1024, NULL, infoLog));
        std::cout << "Error: PROGRAM COMPILE ERROR " << "\n" << infoLog << std::endl;
    }

    //8 清理
    GL_CALL(glDeleteShader(vertex));
    GL_CALL(glDeleteShader(fragment));
}

void render() {
    //如果render最后不解绑vao，这句代码会导致程序异常
    //GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    //执行opengl画布清理操作
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

    //1 绑定当前program
    GL_CALL(glUseProgram(program));

    //2 绑定当前vao
    GL_CALL(glBindVertexArray(vao));

    //3 发出绘制指令
    //GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));
    GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

    //4 解绑当前vao，避免下一帧误操作vao
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

    prepareShader();
    prepareVaoForTriangles();

    while (app->update()) {
        render();
    }

    app->destroy();

    return 0;
}