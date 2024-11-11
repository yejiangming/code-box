cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(OpenGLStudy)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)

# 添加 include 目录
include_directories(/home/wuchujun/helloworld/code-box/opengl-study/include)

# 查找需要的包
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

# 添加源文件
set(SOURCES
    main.cpp
    glad.c
    stb_image.cpp
)

# 添加可执行文件
add_executable(OpenGLStudy ${SOURCES})

# 链接库
target_link_libraries(OpenGLStudy PRIVATE glfw OpenGL::GLU OpenGL::GL Xrandr rt dl)