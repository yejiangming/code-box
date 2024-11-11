#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

GLFWwindow* glfwWindowInit() {
    // 设置 GLFW 创建 openGL 上下文时的参数
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗口对象
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if(window == NULL) {
        std::cout << "Failed to create glfw window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    
    // 通知GLFW将窗口的上下文设置为当前线程的主上下文
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return window;
}

unsigned int createShaderProgramer(const char* fragmentShaderSource) {
    const char *vertexShaderSource = "#version 330 core\n\
    layout (location = 0) in vec3 aPos;\n\
    void main() {\n\
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n\
    }\0";

     // 顶点着色器
    // 创建一个着色器对象，还是使用 id 引用
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // 绑定源码 编译着色器
    // 1 代表源码字符串数量
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    
    std::cout << "vertex shader compile ret:" << success << std::endl;
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "vertex shader compile failed info:" << infoLog << std::endl;
        return 0;
    }

    // 片段着色器
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    
    std::cout << "fragment shader compile ret:" << success << std::endl;
    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "fragment shader compile failed info:" << infoLog << std::endl;
        return 0;
    }

    // 将着色器对象链接到一个用于渲染的着色器程序对象(shader program object)中

    // 上一个着色器的输入会链接到下一个着色器的输入，当输入输出不匹配时会链接错误
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    std::cout << "shader program link ret:" << success << std::endl;
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "shader program link failed info:" << infoLog << std::endl;
        return 0;
    }

    // 激活 shader program object
    glUseProgram(shaderProgram);

    // 着色器链接到 shader program object 后, 删除 shader 对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

unsigned int createTrangle(float delta) {

    // z 坐标代表深度
    float vertices[] = {
        -0.5f+delta, 0.5f, 0.0f,  
        0.0f+delta, 0.0f, 0.0f,  
        -0.5f+delta, 0.0f, 0.0f,
    };

    unsigned int rect_indices[] = {
        0, 1, 2,
    };

    // openGL 核心模式要求使用 VAO
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rect_indices), rect_indices, GL_STATIC_DRAW);

    // 第一个 0 代表将数据传递到 vertex shader layout(location=0) 属性中
    // 3 指定属性大小 vec3 -> 3
    // GLSG 中数据由 float 构成
    // 是否希望数据被标准化(Normalize)
    // stride 步长: 连续的顶点属性组之间的间隔
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    // 每个顶点属性从 VBO 中获取数据
    // 具体从哪个则是由调用 glVertexAttribPointer 时绑定到 GL_ARRAY_BUFFER 的 VBO 决定的
    // 所以顶点属性 0 现在会从之前定义的 VBO 中获取数据

    // 启用顶点属性 0, 默认是禁用的
    glEnableVertexAttribArray(0);

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

    const char *fragmentShaderSource1 = "#version 330 core\n\
    out vec4 FragColor;\n\
    void main() {\n\
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n\
    }";
    unsigned int shaderProgramer1 =  createShaderProgramer(fragmentShaderSource1);
    if (shaderProgramer1 == 0) {
        std::cout << "failed to create shader programer " << std::endl;
        return -1;
    }

    const char *fragmentShaderSource2 = "#version 330 core\n\
    out vec4 FragColor;\n\
    void main() {\n\
        FragColor = vec4(1.0f, 0.5f, 0.5f, 0.5f);\n\
    }";
    unsigned int shaderProgramer2 =  createShaderProgramer(fragmentShaderSource2);
    if (shaderProgramer2 == 0) {
        std::cout << "failed to create shader programer " << std::endl;
        return -1;
    }

    // 设置渲染窗口大小 视口(viewport)
    // 左下角 0, 0
    // width 800
    // height 600
    // 可以讲 openGL 视口设置比 GLFW 小, 这样可以将一些其它元素显示在OpenGL视口之外
    glViewport(0, 0, 800, 600);


    unsigned int VAO1 = createTrangle(0.0f);
    unsigned int VAO2 = createTrangle(0.5f);
    // render loop
    while(!glfwWindowShouldClose(window)) {

        // 输入
        processInput(window);

        // 渲染指令

        // 设置清空屏幕所用的颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 状态设置函数
        glClear(GL_COLOR_BUFFER_BIT); // 状态使用函数

        glUseProgram(shaderProgramer1);
        glBindVertexArray(VAO1);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgramer2);
        glBindVertexArray(VAO2);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        // 检查事件输入(如键盘输入, 鼠标移动)并调用对应的回调函数
        glfwPollEvents();

        // 将颜色缓冲绘制到窗口
        glfwSwapBuffers(window);

    }

    // 释放资源
    glfwTerminate();

    return 0;
}