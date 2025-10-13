file(GLOB_RECURSE FW ./  *.cpp)

add_library(fw ${FW})
target_link_libraries(fw glfw glad glm)
