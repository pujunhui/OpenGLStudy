#include <iostream>

#include <GLFW/glfw3.h>


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
        return false;
    }

    //设置当前窗体对象为OpenGL的绘制舞台
    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window)) {
        //接收并分发窗体消息
        //检查消息队列是否有需要处理的鼠标、键盘等消息
        //如果有的话就将消息批量处理，清空队列
        glfwPollEvents();
    }

    //退出程序前做相关清理
    glfwTerminate();

    return 0;
}
