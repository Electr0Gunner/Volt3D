#include "Shader.hpp"
#include <fstream>
#include <filesystem>
#include <glm/gtc/type_ptr.hpp>

using namespace Smith;


std::string ReadShaderFile(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path))
        throw std::runtime_error("Missing shader: " + path.string());

    std::ifstream file(path);
    return std::string(std::istreambuf_iterator<char>(file), {});
}


Shader::Shader()
{

}

Shader::~Shader()
{
    if (program_ID)
        glDeleteProgram(program_ID);
}

void Shader::Use() const 
{
    glUseProgram(program_ID);
}


bool Shader::LoadFromFile(const std::string& vertPath, const std::string& fragPath)
{
    return LoadFromSource(ReadShaderFile(vertPath), ReadShaderFile(fragPath));
}

bool Shader::LoadFromSource(const std::string& vertSrc, const std::string& fragSrc)
{
    GLuint vertex_shader = CompileShader(GL_VERTEX_SHADER, vertSrc);
    GLuint fragment_shader = CompileShader(GL_FRAGMENT_SHADER, fragSrc); 
    program_ID = glCreateProgram();
    glAttachShader(program_ID, vertex_shader);
    glAttachShader(program_ID, fragment_shader);

    glLinkProgram(program_ID);
    GLint success;
    glGetProgramiv(program_ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        //TODO: same here
        return false;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    
    return true;
}

GLuint Shader::CompileShader(GLenum type, const std::string& source)
{
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    // TODO: ADD FAILURE CASE

    return shader;
}

void Shader::SetInt(const std::string& name, int value) const
{
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetVec4(const std::string& name, const Vector4& value) const
{
    glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::SetMat4(const std::string& name, const Matrix4& value) const
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

GLuint Shader::GetUniformLocation(const std::string& name) const
{
    GLuint pos = glGetUniformLocation(program_ID, name.c_str());
    return pos;
}
