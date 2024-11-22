#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "../include/glm/glm.hpp"

unsigned int createBox(glm::vec3 centor, float halfx, float halfy, float halfz);
void drawBox(unsigned int VAO);
#endif