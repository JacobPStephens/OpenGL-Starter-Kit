#pragma once

#include <map>
#include <string>
#include <glad/glad.h>

class Texture;
class Shader;

class ResourceManager {
    public:
        static std::map<std::string, Shader> shaders;
        static std::map<std::string, Texture> textures;
        static void loadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
        static Shader* getShader(std::string name);
        static void loadTexture(const char* file, bool alpha, std::string name);
        static Texture* getTexture(std::string name);
        static void deleteResources();
    private:
        // empty private constructor because we don't want any resource manager objects
        ResourceManager() {}; 

        static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
        static Texture loadTextureFromFile(const char* file, bool alpha);

};
