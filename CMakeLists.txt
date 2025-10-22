#需求的最低cmake程序版本
cmake_minimum_required(VERSION 3.12)

#本工程的名称
project(OpenGLStudy)

#本工程支持的C++版本
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")

#往项目中加入一个全局的预编译宏
add_definitions(-DDEBUG)

include_directories(.)

#将assets目录下的文件拷贝到可执行目录下
file(GLOB ASSETS 
    "./assets"
    "./thirdParty/assimp/bin/assimp-vc143-mtd.dll"
)
file(COPY ${ASSETS} DESTINATION ${CMAKE_BINARY_DIR})

#将wrapper文件夹纳入到编译系统
add_subdirectory(wrapper)
add_subdirectory(application)
add_subdirectory(glframework)
add_subdirectory(thirdParty)

#本工程所有cpp文件编译链接，生成exe

add_executable(01_window "samples/01_window.cpp")
add_executable(02_events "samples/02_events.cpp")
add_executable(03_glad "samples/03_glad.cpp")
add_executable(04_glError "samples/04_glError.cpp")
add_executable(05_application "samples/05_application.cpp")
add_executable(06_vbo "samples/06_vbo.cpp")
add_executable(07_vao "samples/07_vao.cpp")
add_executable(08_shader "samples/08_shader.cpp")
add_executable(09_drawTriangles "samples/09_drawTriangle.cpp")
add_executable(10_drawMode "samples/10_drawMode.cpp")
add_executable(11_ebo "samples/11_ebo.cpp")
add_executable(12_colorfulTriangle "samples/12_colorfulTriangle.cpp")
add_executable(13_shaderClass "samples/13_shaderClass.cpp")
add_executable(14_uniform "samples/14_uniform.cpp")
add_executable(15_texture "samples/15_texture.cpp")
add_executable(16_texture_sampler "samples/16_texture_sampler.cpp")
add_executable(17_texture_wrap "samples/17_texture_wrap.cpp")
add_executable(18_textureClass "samples/18_textureClass.cpp")
add_executable(19_texture_blending "samples/19_texture_blending.cpp")
add_executable(20_mipmap "samples/20_mipmap.cpp")
add_executable(21_glm "samples/21_glm.cpp")
add_executable(22_transform "samples/22_transform.cpp")
add_executable(23_viewMatrix "samples/23_viewMatrix.cpp")
add_executable(24_projectionMatrix "samples/24_projectionMatrix.cpp")
add_executable(25_camera "samples/25_camera.cpp")
add_executable(26_depthtest "samples/26_depthtest.cpp")
add_executable(27_geometry "samples/27_geometry.cpp")
add_executable(28_normal "samples/28_normal.cpp")
add_executable(29_diffuse "samples/29_diffuse.cpp")
add_executable(30_specular "samples/30_specular.cpp")
add_executable(31_ambient "samples/31_ambient.cpp")
add_executable(32_normalMatrix "samples/32_normalMatrix.cpp")
add_executable(33_struct "samples/33_struct.cpp")
add_executable(34_struct_demo "samples/34_struct_demo.cpp")

#搜索所有的cpp，加入SRCS变量中
#aux_source_directory(. SRCS)

#本工程主程序文件及输出程序名称
#add_executable(glStudy ${SRCS})

target_link_libraries(01_window glfw)
target_link_libraries(02_events glfw)
target_link_libraries(03_glad glfw glad)
target_link_libraries(04_glError glfw glad wrapper)
target_link_libraries(05_application wrapper app)
target_link_libraries(06_vbo wrapper app)
target_link_libraries(07_vao wrapper app)
target_link_libraries(08_shader wrapper app)
target_link_libraries(09_drawTriangles wrapper app)
target_link_libraries(10_drawMode wrapper app)
target_link_libraries(11_ebo wrapper app)
target_link_libraries(12_colorfulTriangle wrapper app)
target_link_libraries(13_shaderClass wrapper app fw)
target_link_libraries(14_uniform wrapper app fw)
target_link_libraries(15_texture wrapper app fw stb)
target_link_libraries(16_texture_sampler wrapper app fw stb)
target_link_libraries(17_texture_wrap wrapper app fw stb)
target_link_libraries(18_textureClass wrapper app fw)
target_link_libraries(19_texture_blending wrapper app fw)
target_link_libraries(20_mipmap wrapper app fw)
target_link_libraries(21_glm wrapper app fw)
target_link_libraries(22_transform wrapper app fw)
target_link_libraries(23_viewMatrix wrapper app fw)
target_link_libraries(24_projectionMatrix wrapper app fw)
target_link_libraries(25_camera wrapper app fw)
target_link_libraries(26_depthtest wrapper app fw)
target_link_libraries(27_geometry wrapper app fw)
target_link_libraries(28_normal wrapper app fw)
target_link_libraries(29_diffuse wrapper app fw)
target_link_libraries(30_specular wrapper app fw)
target_link_libraries(31_ambient wrapper app fw)
target_link_libraries(32_normalMatrix wrapper app fw)
target_link_libraries(33_struct wrapper app fw)
target_link_libraries(34_struct_demo wrapper app fw)
