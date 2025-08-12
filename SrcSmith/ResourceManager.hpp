#pragma once

#include "graphics/Texture.hpp"
#include "Common.hpp"

#include <memory>
#include <map>

namespace Smith
{
    class ResourceManager
    {
    private:
        static std::map<std::string, std::shared_ptr<Texture>> texture_cache;
    public:
        static void Init();
        static Texture* GetTexture(const std::string& id);
        static Texture* LoadTexture(const std::string& path);
        static Texture* GetMissingTexture();
    };
    
} // namespace Smith
