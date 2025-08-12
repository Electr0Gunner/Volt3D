#pragma once

#include "Common.hpp"

#include <string>
#include <glad/glad.h>

namespace Smith
{
    class Shader
    {
    public:
        Shader();
        ~Shader();

        void Use() const;
        GLuint GetID() const {return program_ID;}

        void SetInt(const std::string& name, int value) const;
        void SetVec4(const std::string& name, const Vector4& value) const;
        void SetMat4(const std::string& name, const Matrix4& value) const;

        GLuint GetUniformLocation(const std::string& name) const;

        bool LoadFromFile(const std::string& vertPath, const std::string& fragPath);
        bool LoadFromSource(const std::string& vertSrc, const std::string& fragSrc);

    private:
        GLuint CompileShader(GLenum type, const std::string& source);

        GLuint program_ID;
    };
} // namespace Smith
