cmake_minimum_required(VERSION 3.12)

# 设置第三方库的路径
set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# 创建 glfw 库
add_library(glfw STATIC IMPORTED GLOBAL)
set_target_properties(glfw PROPERTIES
    IMPORTED_LOCATION "${THIRD_PARTY_DIR}/GLFW/lib/glfw3.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${THIRD_PARTY_DIR}/GLFW/include"
)

# 创建 glad 库
add_library(glad STATIC ${THIRD_PARTY_DIR}/glad/src/glad.c)
target_include_directories(glad PUBLIC "${THIRD_PARTY_DIR}/glad/include")

# 创建 glm 库
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE "${THIRD_PARTY_DIR}/glm")

# 创建 stb 库
add_library(stb STATIC ${THIRD_PARTY_DIR}/stb/stb_image.cpp)
target_include_directories(stb PUBLIC "${THIRD_PARTY_DIR}/stb")

# 创建 assimp 库
add_library(assimp STATIC IMPORTED GLOBAL)
set_target_properties(assimp PROPERTIES
    IMPORTED_LOCATION "${THIRD_PARTY_DIR}/assimp/lib/assimp-vc143-mtd.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${THIRD_PARTY_DIR}/assimp/include"
)
