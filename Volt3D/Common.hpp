#pragma once

#include <string>
#include <glm/glm.hpp>

#ifndef VOLT3D_VERSION_MAJOR
#define VOLT3D_VERSION_MAJOR 0
#endif

#ifndef VOLT3D_VERSION_MINOR
#define VOLT3D_VERSION_MINOR 0
#endif

#ifndef VOLT3D_VERSION_PATCH
#define VOLT3D_VERSION_PATCH 0
#endif

#ifndef VOLT3D_VERSION_STAGE
#define VOLT3D_VERSION_STAGE unknown
#endif

#define STR_HELPER(x) #x
#define STR_MACRO(x) STR_HELPER(x)


#ifdef _DEBUG
#define VOLT3D_BUILD_TYPE STR_HELPER(debug)
#else
#define VOLT3D_BUILD_TYPE STR_HELPER(release)
#endif


#define VOLT3D_VERSION STR_MACRO(VOLT3D_VERSION_MAJOR) "." STR_MACRO(VOLT3D_VERSION_MINOR) "." STR_MACRO(VOLT3D_VERSION_PATCH) "-" STR_MACRO(VOLT3D_VERSION_STAGE)

namespace Volt
{
    typedef glm::mat2 Matrix2;
    typedef glm::mat3 Matrix3;
    typedef glm::mat4 Matrix4;
    typedef glm::vec2 Vector2;
    typedef glm::vec3 Vector3;
    typedef glm::vec4 Vector4;

    inline std::string Trim(const std::string& str)
    {
        std::string s(str);
        s.erase(0, s.find_first_not_of(" \n\r\t"));
        s.erase(s.find_last_not_of(" \n\r\t")+1);
        return s;
    }
}
