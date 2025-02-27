#include "geometry.h"

#include <vector>
#include <cmath>

Geometry::Geometry() {

}

Geometry::Geometry(
    const std::vector<float>& positions,
    const std::vector<float>& normals,
    const std::vector<float>& uvs,
    const std::vector<unsigned int>& indices
) {
    mIndicesCount = indices.size();

    glGenBuffers(1, &mPosVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mPosVbo);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &mUvVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mUvVbo);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &mNormalVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mNormalVbo);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &mEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &mVao);
    glBindVertexArray(mVao);

    glBindBuffer(GL_ARRAY_BUFFER, mPosVbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

    glBindBuffer(GL_ARRAY_BUFFER, mUvVbo);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    glBindBuffer(GL_ARRAY_BUFFER, mNormalVbo);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);

    glBindVertexArray(0);
}

Geometry::~Geometry() {
    if (mVao != 0)
        glDeleteVertexArrays(1, &mVao);
    if (mPosVbo != 0)
        glDeleteBuffers(1, &mPosVbo);
    if (mUvVbo != 0)
        glDeleteBuffers(1, &mUvVbo);
    if (mNormalVbo != 0)
        glDeleteBuffers(1, &mNormalVbo);
    if (mEbo != 0)
        glDeleteBuffers(1, &mEbo);
}

Geometry* Geometry::createBox(float size) {
    Geometry* geometry = new Geometry();
    geometry->mIndicesCount = 36;
    float halfSize = size / 2.0f;

    float positions[] = {
        // Front face
        -halfSize, -halfSize,  halfSize,
         halfSize, -halfSize,  halfSize,
         halfSize,  halfSize,  halfSize,
        -halfSize,  halfSize,  halfSize,
        // Back face
        -halfSize, -halfSize, -halfSize,
         halfSize, -halfSize, -halfSize,
         halfSize,  halfSize, -halfSize,
        -halfSize,  halfSize, -halfSize,
        // Left face
        -halfSize, -halfSize, -halfSize,
        -halfSize,  halfSize, -halfSize,
        -halfSize,  halfSize,  halfSize,
        -halfSize, -halfSize,  halfSize,
        // Right face
         halfSize, -halfSize, -halfSize,
         halfSize,  halfSize, -halfSize,
         halfSize,  halfSize,  halfSize,
         halfSize, -halfSize,  halfSize,
         // Top face
         -halfSize,  halfSize, -halfSize,
          halfSize,  halfSize, -halfSize,
          halfSize,  halfSize,  halfSize,
         -halfSize,  halfSize,  halfSize,
         // Bottom face
         -halfSize, -halfSize, -halfSize,
          halfSize, -halfSize, -halfSize,
          halfSize, -halfSize,  halfSize,
         -halfSize, -halfSize,  halfSize
    };

    float uvs[] = {
        // Front face
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // Back face
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // Left face
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // Right face
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // Top face
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // Bottom face
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    float normals[] = {
        //Front face
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        //Back face
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,

        //Left face
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,

        //Right face
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        //Top face
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        //Bottom face
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
    };

    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0, // front face
        4, 5, 6, 6, 7, 4, // back face
        8, 9, 10, 10, 11, 8, // left face
        12, 13, 14, 14, 15, 12, // right face
        16, 17, 18, 18, 19, 16, // top face
        20, 21, 22, 22, 23, 20  // bottom face
    };

    GLuint& posVbo = geometry->mPosVbo, uvVbo = geometry->mUvVbo, normalVbo = geometry->mNormalVbo;
    glGenBuffers(1, &posVbo);
    glBindBuffer(GL_ARRAY_BUFFER, posVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glGenBuffers(1, &uvVbo);
    glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

    glGenBuffers(1, &normalVbo);
    glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

    glGenBuffers(1, &geometry->mEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &geometry->mVao);
    glBindVertexArray(geometry->mVao);

    glBindBuffer(GL_ARRAY_BUFFER, posVbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

    glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);

    glBindVertexArray(0);
    return geometry;
}

Geometry* Geometry::createSphere(float radius) {
    Geometry* geometry = new Geometry();

    std::vector<GLfloat> positions{};
    std::vector<GLfloat> uvs{};
    std::vector<GLuint> indices{};
    std::vector<GLfloat> normals{};

    int numLatLines = 60; //latitude 纬线数
    int numLongLines = 60; //longitude 经线数

    // position, uv
    for (int i = 0; i <= numLatLines; i++) {    // 枚举纬线(生成多余点, 便于定点交织)
        float phi = i * glm::pi<float>() / numLatLines; //经线角
        for (int j = 0; j <= numLongLines; j++) {   // 枚举经线
            float theta = j * 2 * glm::pi<float>() / numLongLines; //纬线角

            float y = radius * cos(phi);
            float x = radius * sin(phi) * cos(theta);
            float z = radius * sin(phi) * sin(theta);

            positions.push_back(x);
            positions.push_back(y);
            positions.push_back(z);

            float u = 1.0f - (float)j / (float)numLongLines;
            float v = 1.0f - (float)i / (float)numLatLines;

            uvs.push_back(u);
            uvs.push_back(v);

            // 未归一化, 在shader中归一化
            normals.push_back(x);
            normals.push_back(y);
            normals.push_back(z);
        }
    }

    // indices
    for (int i = 0; i < numLatLines; i++) {
        for (int j = 0; j < numLongLines; j++) {
            int p1 = i * (numLongLines + 1) + j;    // +1: 边界冗余点
            int p2 = p1 + numLongLines + 1;
            int p3 = p1 + 1;
            int p4 = p2 + 1;

            indices.push_back(p1);
            indices.push_back(p2);
            indices.push_back(p3);
            indices.push_back(p3);
            indices.push_back(p2);
            indices.push_back(p4);
        }
    }

    GLuint& posVbo = geometry->mPosVbo, uvVbo = geometry->mUvVbo, normalVbo = geometry->mNormalVbo;
    glGenBuffers(1, &posVbo);
    glBindBuffer(GL_ARRAY_BUFFER, posVbo);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &uvVbo);
    glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &normalVbo);
    glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &geometry->mEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &geometry->mVao);
    glBindVertexArray(geometry->mVao);

    glBindBuffer(GL_ARRAY_BUFFER, posVbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

    glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);

    glBindVertexArray(0);

    geometry->mIndicesCount = indices.size();
    return geometry;
}

Geometry* Geometry::createPlane(float width, float height) {
    Geometry* geometry = new Geometry();
    geometry->mIndicesCount = 6;

    float halfW = width / 2.0f;
    float halfH = height / 2.0f;

    float positions[] = {
        -halfW, -halfH, 0.0f,
        halfW, -halfH, 0.0f,
        halfW, halfH, 0.0f,
        -halfW, halfH, 0.0f,
    };

    float uvs[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    float normals[] = {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };


    GLuint& posVbo = geometry->mPosVbo, & uvVbo = geometry->mUvVbo, & normalVbo = geometry->mNormalVbo;
    glGenBuffers(1, &posVbo);
    glBindBuffer(GL_ARRAY_BUFFER, posVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glGenBuffers(1, &uvVbo);
    glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

    glGenBuffers(1, &normalVbo);
    glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

    glGenBuffers(1, &geometry->mEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glGenVertexArrays(1, &geometry->mVao);
    glBindVertexArray(geometry->mVao);

    glBindBuffer(GL_ARRAY_BUFFER, posVbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);

    glBindVertexArray(0);

    return geometry;
}

Geometry* Geometry::createScreenPlane() {
    Geometry* geometry = new Geometry();
    geometry->mIndicesCount = 6;

    float positions[] = {
        -1.0f, 1.0f,
         -1.0f, -1.0f,
         1.0f,  -1.0f,
        1.0f,  1.0f,
    };

    float uvs[] = {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    GLuint& posVbo = geometry->mPosVbo, uvVbo = geometry->mUvVbo;
    glGenBuffers(1, &posVbo);
    glBindBuffer(GL_ARRAY_BUFFER, posVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glGenBuffers(1, &uvVbo);
    glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

    glGenBuffers(1, &geometry->mEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &geometry->mVao);
    glBindVertexArray(geometry->mVao);

    glBindBuffer(GL_ARRAY_BUFFER, posVbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);

    glBindVertexArray(0);
    return geometry;
}