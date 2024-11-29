#include "../include/glad/glad.h"
#include "shader.h"
#include "camera.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "stb_image.h"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"
#include "geometry.h"
#include "sample_camera.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Camera camera(glm::vec3(-20.0f, 5.0f, 0.0f));

float deltaTime = 0.0f;

bool firstMouse = true;
// 设置初始的鼠标位置
float lastX = 400, lastY = 300;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// xpos, ypos 代表鼠标位置
// 鼠标越往右 xpos 越大, 越往下 ypos 越大
// void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
//     // std::cout << "xpos:" << xpos << " " << "ypos:" << ypos << std::endl;
//     float xpos = static_cast<float>(xposIn);
//     float ypos = static_cast<float>(yposIn);

//     if(firstMouse) {
//         lastX = xpos;
//         lastY = ypos;
//         firstMouse = false;
//     }

//     float xoffset = xpos - lastX;
//     float yoffset = lastY - ypos;
    
//     lastX = xpos;
//     lastY = ypos;

//     camera.ProcessMouseMovement(xoffset, yoffset);
// }

// void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
//     camera.ProcessMouseScroll(static_cast<float>(yoffset));
// }

GLFWwindow* glfwWindowInit() {
    // 设置 GLFW 创建 openGL 上下文时的参数
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗口对象
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if(window == NULL) {
        std::cout << "Failed to create glfw window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    
    // 通知GLFW将窗口的上下文设置为当前线程的主上下文
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glfwSetCursorPosCallback(window, mouse_callback);
    // glfwSetScrollCallback(window, scroll_callback);

    return window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    //     camera.ProcessKeyboard(FORWARD, deltaTime);
    // }
    // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    //     camera.ProcessKeyboard(BACKWARD, deltaTime);
    // }
    // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    //     camera.ProcessKeyboard(LEFT, deltaTime);
    // }
    // if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    //     camera.ProcessKeyboard(RIGHT, deltaTime);
    // }
}

unsigned int createTrangle(float delta) {

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // unsigned int indices[] = {
    //     0, 1, 2, 2, 3, 0
    // };

    // openGL 核心模式要求使用 VAO
    unsigned int VAO, VBO;
    // unsigned int EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // 把新创建的缓冲绑定到 GL_ARRAY_BUFFER 缓冲类型上
    // openGL 有很多缓冲对象类型
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 将用户数据复制到当前绑定缓冲
    // GL_ARRAY_BUFFER 目标缓冲类型, VBO 现在绑定在 GL_ARRAY_BUFFER 上
    // sizeof(vertices) 传输的数据大小
    // vertices 数据
    // GL_STATIC_DRAW 指定显卡如何管理给定的数据, 这个值表明数据几乎不变(符合顶点数据性质)

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 第一个 0 代表将数据传递到 vertex shader layout(location=0) 属性中
    // 属性值有 3 * float 个数据那么大
    // 是否希望数据被标准化(Normalize)
    // stride 步长: 连续的位置点之间的间隔
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    // 每个顶点属性从 VBO 中获取数据
    // 具体从哪个则是由调用 glVertexAttribPointer 时绑定到 GL_ARRAY_BUFFER 的 VBO 决定的
    // 所以顶点属性 0 现在会从之前定义的 VBO 中获取数据

    // 启用顶点属性 0, 默认是禁用的
    glEnableVertexAttribArray(0);

    // 颜色属性
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    // glEnableVertexAttribArray(1);

    // UV 属性
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // 解绑 VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // 解绑 VAO
    glBindVertexArray(0);

    // 使用当前激活的着色器,之前定义的顶点属性配置, VBO 顶点数据绘制
    // 绘制三角形
    // 顶点数组起始索引 0
    // 绘制 3 个点
    // glDrawArrays(GL_TRIANGLES, 0, 3);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    return VAO;
}

unsigned int createTexture(const char* filePath) {
// 读取图片
    int width, height, nrChannels;
    // 加载图像时翻转 y 轴, 因为图像的 y 轴 0.0 在顶部, 而 openGL uv 坐标中的 0 代表底部
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filePath, &width, &height, &nrChannels, 0);
    
    if (!data) {
        std::cout << "Fail to read image" << std::endl;
    }

    GLenum format;
    if (nrChannels == 1){
        format = GL_RED;
    } else if (nrChannels == 3) {
        format = GL_RGB;
    } else if (nrChannels == 4) {
        format = GL_RGBA;
    }

    std::cout << "fileName: " << filePath << " format: " << std::hex << format << std::endl;
    // 生成纹理
    unsigned int texture;
    glGenTextures(1, &texture);

    // 绑定纹理, 之后的纹理指令都可以配置当前绑定的纹理
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINE);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    // 自动生成多级渐远纹理    
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    return texture;
}

// g++ -I /home/wuchujun/helloworld/code-box/opengl-study/include main.cpp glad.c -lglfw -lGLU -lGL -lXrandr  -lrt -ldl
int main()
{
    GLFWwindow* window = glfwWindowInit();
    if(window == NULL) {
        std::cout << "Failed to initialize glfw window" << std::endl;
        return -1;
    }

    // 初始化 glad
    // glfwGetProcAddress 是 GLFW 提供的一个函数，用于获取 OpenGL 函数的地址
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 启用深度测试
    glEnable(GL_DEPTH_TEST);

    Shader boxShader("../shader/box.vs", 
    "../shader/box.fs");

    // Shader boxShader("../shader/box.vs", 
    // "../shader/box.fs");

    Shader lightShader("../shader/light.vs",
    "../shader/light.fs"); 

    // 设置渲染窗口大小 视口(viewport)
    // 左下角 0, 0
    // width 800
    // height 600
    // 可以讲 openGL 视口设置比 GLFW 小, 这样可以将一些其它元素显示在OpenGL视口之外
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    SampleCamera sampleCamera(glm::vec3(-4.0f, 3.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(4.0f, -3.0f, 0.0f),
    75.0f, (4.0f/3.0f), 0.1f, 1000.0f);

    glm::vec3 boxCentor(0.0f, 0.0f, 0.0f);
    unsigned int box1 = createBox(boxCentor, 1.0f, 1.0f, 1.0f);

    // glm::vec3 lightCentor(0.0f, 4.0f, 4.0f);
    glm::vec3 lightCentor(0.0f, 2.0f, 0.0f);
    unsigned int light1 = createBox(lightCentor, 0.1f, 0.1f, 0.1f);

    // unsigned int VAO1 = createTrangle(0.0f);
    // unsigned int VAO2 = createTrangle(0.5f);

    unsigned int textureDifussion = createTexture("../texture/container2.png");
    unsigned int textureSpecular = createTexture("../texture/container2_specular.png");
    unsigned int textureEmission = createTexture("../texture/matrix.jpg");
    // shaderProgramer.setInt("texture2", 1);
    
    // render loop
    while(!glfwWindowShouldClose(window)) {

        float startTime = (float)glfwGetTime();

        // 输入
        processInput(window);

        // 渲染指令

        // 设置清空屏幕所用的颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 状态设置函数

        // 在没一帧前清理 颜色缓冲 深度缓冲
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        // shaderProgramer.setFloat("mixValue", mixValue);
        // glBindVertexArray(box1);

        // lightModel = glm::translate(lightModel, glm::vec3(sin(startTime), 0.0, cos(startTime)));

        glm::mat4 boxModel = glm::mat4(1.0f);
        // float angle = glm::radians(startTime*10); 
        // glm::vec3 axis = glm::vec3(0, 1, 0); 
        // boxModel = glm::rotate(boxModel, angle, axis);

        glm::mat4 lightModel = glm::mat4(1.0f);

        boxShader.use();

        boxShader.setMat4f("model", glm::value_ptr(boxModel));

        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        // lightColor.x = sin(startTime * 2.0f);
        // lightColor.y = sin(startTime * 0.7f);
        // lightColor.z = sin(startTime * 1.3f);

        glm::vec3 lAmbient = lightColor * glm::vec3(0.2f);
        glm::vec3 lDiffuse = lightColor * glm::vec3(0.5f);
        glm::vec3 lSpecular = lightColor * glm::vec3(1.0f);

        // glm::vec3 lAmbient = lightColor * glm::vec3(1.0f);
        // glm::vec3 lDiffuse = lightColor * glm::vec3(1.0f);
        // glm::vec3 lSpecular = lightColor * glm::vec3(1.0f);

        // glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 view = sampleCamera.GetViewMatrix();
        boxShader.setMat4f("view", glm::value_ptr(view));

        // glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), float(SCR_WIDTH/SCR_HEIGHT), 0.1f, 1000.0f);
        glm::mat4 projection = sampleCamera.GetPerspectiveMatrix();
        boxShader.setMat4f("projection", glm::value_ptr(projection));

        boxShader.setVec3f("lightColor", glm::value_ptr(lightColor));
        glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.3f);
        boxShader.setVec3f("objectColor", glm::value_ptr(objectColor));

        lightModel = glm::translate(lightModel, glm::vec3(cos(glm::radians(startTime*30)), 0, sin(glm::radians(startTime*30))));
        glm::vec3 worldLightPos =  glm::vec3(lightModel * glm::vec4(lightCentor, 1.0f));
        // glm::vec3 viewLightPos = glm::vec3(view * glm::vec4(worldLightPos, 1.0f));

        boxShader.setVec3f("lightPos", glm::value_ptr(worldLightPos));
        // boxShader.setVec3f("lightPos", glm::value_ptr(viewLightPos));

        boxShader.setVec3f("viewPos", glm::value_ptr(sampleCamera.Position));

        glm::vec3 mAmbient(0.24725f,0.1995f,0.0745f);
        glm::vec3 mDiffuse(0.75164f,0.60648f,0.22648f);
        glm::vec3 mSpecular(0.628281f, 0.555802f, 0.366065f);
        float shininess = 32;

        boxShader.setVec3f("material.ambient",  glm::value_ptr(mAmbient));
        boxShader.setVec3f("material.diffuse",  glm::value_ptr(mDiffuse));
        boxShader.setVec3f("material.specular", glm::value_ptr(mSpecular));
        boxShader.setFloat("material.shininess", shininess);

        boxShader.setVec3f("light.ambient",  glm::value_ptr(lAmbient));
        boxShader.setVec3f("light.diffuse",  glm::value_ptr(lDiffuse));
        boxShader.setVec3f("light.specular", glm::value_ptr(lSpecular));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureDifussion);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureSpecular);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, textureEmission);
        boxShader.setInt("material.diffuse", 0);
        boxShader.setInt("material.specular", 1);
        boxShader.setInt("material.emission", 2);
        boxShader.setFloat("material.movement", startTime);

        drawBox(box1);

        lightShader.use();
        lightShader.setMat4f("projection", glm::value_ptr(projection));
        lightShader.setMat4f("view", glm::value_ptr(view));
        lightShader.setMat4f("model", glm::value_ptr(lightModel));
        lightShader.setVec3f("lightColor", glm::value_ptr(lightColor));

        drawBox(light1);
        // glDrawArrays(GL_TRIANGLES, 0, 36);

        // glUseProgram(shaderProgramer2);
        // glBindVertexArray(VAO2);
        // glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        // 检查事件输入(如键盘输入, 鼠标移动)并调用对应的回调函数
        glfwPollEvents();

        // 将颜色缓冲绘制到窗口
        glfwSwapBuffers(window);

        float endTime = glfwGetTime();
        deltaTime = endTime - startTime;
        // std::cout << "deltaTime: " << deltaTime << std::endl;

    }

    // 释放资源
    glfwTerminate();

    return 0;
}