#include "ResourceManager.hpp"
#include <filesystem>

using namespace Volt;

std::map<std::string, Texture*> Volt::ResourceManager::texture_cache;

Texture* ResourceManager::GetTexture(const std::string& id)
{
    for (const auto& [key, value] : texture_cache)
    if (key == id)
        return value;
    return LoadTexture("data/textures/" + id);
}

Texture* ResourceManager::LoadTexture(const std::string& path)
{
    if (!std::filesystem::exists(path))
        return nullptr;

    return nullptr; //temp
}