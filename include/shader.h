//
// Created by Cody Perry on 8/26/26.
//

#ifndef SHADER_H
#define SHADER_H
#include <filesystem>

class Shader
{
public:
    Shader(std::filesystem::path vertexShaderPath, std::filesystem::path fragmentShaderPath);
    bool load();
    bool compile_and_link();
    void use();
private:
    unsigned int m_id;
};

#endif //SHADER_H
