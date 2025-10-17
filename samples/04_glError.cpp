#include <iostream>

//注意：glad头文件必须在glfw引用之前引用
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "wrapper/checkError.h"

//声明且实现一个响应窗体大小变化的函数
void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    //std::cout << "窗体最新大小：" << width << ", " << height << std::endl;
    glViewport(0, 0, width, height);
}

//声明且实现一个键盘消息回调函数
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    std::cout << "按下了：" << key << std::endl;
    std::cout << "action：" << action << std::endl;
    std::cout << "mods：" << mods << std::endl;
}

int main() {
    //1 初始化GLFW基本环境
    glfwInit();
    //1.1 设置OpenGL主版本号、次版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    //1.2 设置OpenGL启用核心模式（非立即渲染模式）
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //2 创建窗体对象
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGLStudy", NULL, NULL);
    if (window == NULL) {
        return -1;
    }

    //设置当前窗体对象为OpenGL的绘制舞台
    glfwMakeContextCurrent(window);

    //加载glad,完成后才能使用opengl函数
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //窗口大小改变回调
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    //键盘回调
    glfwSetKeyCallback(window, keyCallback);

    //设置opengl视口以及清理颜色
    glViewport(0, 0, 800, 600);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        //接收并分发窗体消息
        //检查消息队列是否有需要处理的鼠标、键盘等消息
        //如果有的话就将消息批量处理，清空队列
        glfwPollEvents();

        //执行opengl画布清理操作
        GL_CALL(glClear(-1));

        //渲染操作
        
        //切换双缓存
        glfwSwapBuffers(window);
    }

    //退出程序前做相关清理
    glfwTerminate();

    return 0;
}
