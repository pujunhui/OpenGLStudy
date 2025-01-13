#需求的最低cmake程序版本
cmake_minimum_required(VERSION 3.12)

#本工程的名称
project(OpenGLStudy)

#本工程支持的C++版本
set(CMAKE_CXX_STANDARD 17)

#往项目中加入一个全局的预编译宏
add_definitions(-DDEBUG)

#把需要拷贝的资源路径都放到ASSETS里
file(GLOB ASSETS "./assets" ${GLFW})

#把ASSETS指代的目录集合内容，都拷贝到可执行文件目录下
file(COPY ${ASSETS} DESTINATION ${CMAKE_BINARY_DIR})

include_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/include
)

link_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/lib
)

#将wrapper文件夹纳入到编译系统
add_subdirectory(wrapper)
add_subdirectory(application)
add_subdirectory(glframework)

#本工程所有cpp文件编译链接，生成exe
add_executable(main "main.cpp" "glad.c")

add_executable(main_mipmap "main_mipmap.cpp" "glad.c")

#搜索所有的cpp，加入SRCS变量中
#aux_source_directory(. SRCS)

#本工程主程序文件及输出程序名称
#add_executable(glStudy ${SRCS})

target_link_libraries(main glfw3.lib wrapper app fw)
target_link_libraries(main_mipmap glfw3.lib wrapper app fw)

