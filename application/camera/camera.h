#pragma once 

#include "../../glframework/core.h"

class Camera {
public:
    glm::mat4 getViewMatrix();
    virtual glm::mat4 getProjectionMatrix() = 0;

public:
    glm::vec3 mUp{ 0.0f, 1.0f, 0.0f };
    glm::vec3 mRight{ 1.0f, 0.0f, 0.0f };
    glm::vec3 mPosition{ 0.0f, 0.0f, 2.0f };
};
