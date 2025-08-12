#pragma once

#include <string>
#include <glm/glm.hpp>

#ifndef SRCSMITH_VERSION_MAJOR
#define SRCSMITH_VERSION_MAJOR 0
#endif

#ifndef SRCSMITH_VERSION_MINOR
#define SRCSMITH_VERSION_MINOR 0
#endif

#ifndef SRCSMITH_VERSION_PATCH
#define SRCSMITH_VERSION_PATCH 0
#endif

#ifndef SRCSMITH_VERSION_STAGE
#define SRCSMITH_VERSION_STAGE unknown
#endif

#define STR_HELPER(x) #x
#define STR_MACRO(x) STR_HELPER(x)


#ifdef _DEBUG
#define SRCSMITH_BUILD_TYPE STR_HELPER(debug)
#else
#define SRCSMITH_BUILD_TYPE STR_HELPER(release)
#endif


#define SRCSMITH_VERSION STR_MACRO(SRCSMITH_VERSION_MAJOR) "." STR_MACRO(SRCSMITH_VERSION_MINOR) "." STR_MACRO(SRCSMITH_VERSION_PATCH) "-" STR_MACRO(SRCSMITH_VERSION_STAGE)

namespace Smith
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
