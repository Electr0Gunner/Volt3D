#pragma once

#include <App.hpp>

namespace Anvil
{
    #ifndef ANVIL_VERSION_MAJOR
    #define ANVIL_VERSION_MAJOR 0
    #endif

    #ifndef ANVIL_VERSION_MINOR
    #define ANVIL_VERSION_MINOR 0
    #endif

    #ifndef ANVIL_VERSION_PATCH
    #define ANVIL_VERSION_PATCH 0
    #endif

    #ifndef ANVIL_VERSION_STAGE
    #define ANVIL_VERSION_STAGE unknown
    #endif

    #define STR_HELPER(x) #x
    #define STR_MACRO(x) STR_HELPER(x)


    #ifdef _DEBUG
    #define ANVIL_BUILD_TYPE STR_HELPER(debug)
    #else
    #define ANVIL_BUILD_TYPE STR_HELPER(release)
    #endif


    #define ANVIL_VERSION STR_MACRO(ANVIL_VERSION_MAJOR) "." STR_MACRO(ANVIL_VERSION_MINOR) "." STR_MACRO(ANVIL_VERSION_PATCH) "-" STR_MACRO(ANVIL_VERSION_STAGE)

    struct MapRawFile
    {
        
    };

    class Editor : public Smith::App
    {
    private:
        /* data */
    public:
        Editor(/* args */);
        ~Editor();

        void Init();
    };

} // namespace Anvil
