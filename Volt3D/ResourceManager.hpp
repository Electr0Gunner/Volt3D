#pragma once

#include "graphics/Texture.hpp"
#include "Common.hpp"

#include <map>

namespace Volt
{
    class ResourceManager
    {
    private:
        static std::map<std::string, Texture*> texture_cache;
    public:
        static Texture* GetTexture(const std::string& id);
        static Texture* LoadTexture(const std::string& path);

    };
    
} // namespace Volt
