# A starter kit for OpenGL development

A barebones engine for games and software using OpenGL and C++. To get you started, contains

    - A demo object rendered to the screen with a position, size, rotation, and texture. 
    - Basic input handling of keyboard and mouse 

This branch is a more comprehensive version of the original starter kit. This branch includes the 
following classes: 

    - App
    - Object
    - Renderer
    - ResourceManager
    - Shader
    - Texture

To build the project, run cmake --build build. This will create an executable "app" located at build/src/app.

Additional resources on included libraries:
- OpenGL https://learnopengl.com/, https://open.gl
- GLFW https://www.glfw.org/
- CMake https://cmake.org/cmake/help/latest/guide/tutorial/index.html
- glad https://glad.dav1d.de/
- stb https://github.com/nothings/stb

glad version 3.3 core
glm version 1.0.3
glfw version 3.4 
stb version 2.30
