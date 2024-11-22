#ifndef SAMPLE_CAMERA_H
#define SAMPLE_CAMERA_H

#include <GLFW/glfw3.h>
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class SampleCamera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    
    float Fov;
    float Aspect;
    float Near;
    float Far;

    // constructor with vectors
    SampleCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f),
    float fov = 45.0f, float aspect = 4.0f/3.0f, float near = 0.1f, float far = 1000.0f)
    {
        Position = position;
        WorldUp = up;
        Front = front;

        Fov = fov;
        Aspect = aspect;
        Near = near;
        Far = far;

        // 计算出相机的 front up right
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    glm::mat4 GetPerspectiveMatrix()
    {
        return glm::perspective(glm::radians(Fov), Aspect, Near, Far);
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        Front = glm::normalize(Front);
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};
#endif