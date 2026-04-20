#include "ResourceManager.h"
#include "Texture.h"
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <sstream>
#include <fstream>

std::map<std::string, Texture> ResourceManager::textures;
std::map<std::string, Shader> ResourceManager::shaders;

void ResourceManager::loadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name) {
    shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
}

Shader* ResourceManager::getShader(std::string name) {
    return &shaders.at(name);
}

void ResourceManager::loadTexture(const char* file, bool alpha, std::string name) {
    textures[name] = loadTextureFromFile(file, alpha);
}

Texture* ResourceManager::getTexture(std::string name) {
    return &textures.at(name);
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile) {
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;

    try {
        // open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // read file bytes into stream variables
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        // do the same for geometry shader, if exists
        if (gShaderFile != nullptr) {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e) {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();

    // now create shader from source code files
    Shader shader;
    shader.compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);

    return shader;
}

Texture ResourceManager::loadTextureFromFile(const char* file, bool alpha) {

    // initialize texture object
    Texture texture;
    if (alpha) {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }

    // load image using stbi
    int width, height, nChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nChannels, 0);

    // generate texture using data
    texture.Generate(width, height, data);

    stbi_image_free(data);
    return texture;

}

void ResourceManager::deleteResources() {

    for (auto iter: shaders)
        glDeleteProgram(iter.second.shaderProgramID);
    for (auto iter : textures)
        glDeleteTextures(1, &iter.second.ID);
 
}