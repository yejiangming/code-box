#include "../include/glad/glad.h"
#include "../include/glm/glm.hpp"
#include <vector>
#include <iostream>

unsigned int createBox(glm::vec3 centor, float halfx, float halfy, float halfz) {
    
    std::vector<float> vertices;

    float x_direction[] = {
        -1, 1, 1, 1, -1, -1,
        -1, 1, 1, 1, -1, -1,
        -1, -1, -1, -1, -1, -1,
        1, 1, 1, 1, 1, 1,
        -1, 1, 1, 1, -1, -1, 
        -1, 1, 1, 1, -1, -1
    };

    float y_direction[] = {
        -1, -1, 1, 1, 1, -1,
        -1, -1, 1, 1, 1, -1,
        1, 1, -1, -1, -1, 1,
        1, 1, -1, -1, -1, 1,
        -1, -1, -1, -1, -1, -1,
        1, 1, 1, 1, 1, 1
    };

    float z_direction[] = {
        -1, -1, -1, -1, -1, -1,
        1, 1, 1, 1, 1, 1,
        1, -1, -1, -1, 1, 1,
        1, -1, -1, -1, 1, 1,
        -1, -1, 1, 1, 1, -1,
        -1, -1, 1, 1, 1, -1
    };

    float norms[] = {
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f, 
        0.0f,  0.0f, -1.0f, 
        0.0f,  0.0f, -1.0f, 
        0.0f,  0.0f, -1.0f, 
        0.0f,  0.0f, -1.0f, 

        0.0f,  0.0f, 1.0f,
        0.0f,  0.0f, 1.0f,
        0.0f,  0.0f, 1.0f,
        0.0f,  0.0f, 1.0f,
        0.0f,  0.0f, 1.0f,
        0.0f,  0.0f, 1.0f,

        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,

        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,

        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f,

        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f
    };

    float textureCoords[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f
    };

    for(int i = 0; i < 36; i++) {
        float x_d = x_direction[i];
        float y_d = y_direction[i];
        float z_d = z_direction[i];
        
        vertices.push_back(centor.x + x_d * halfx);
        vertices.push_back(centor.y + y_d * halfy);
        vertices.push_back(centor.z + z_d * halfz);
    }

    unsigned int VAO;
    unsigned int VBO;
    unsigned int NBO;
    unsigned int TBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &NBO);
    glGenBuffers(1, &TBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 6*6*3*4, vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, 6*6*3*4, norms, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glBufferData(GL_ARRAY_BUFFER, 6*6*2*4, textureCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;

}

void drawBox(unsigned int VAO) {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}