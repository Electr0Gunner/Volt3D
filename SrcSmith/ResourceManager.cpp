#include "ResourceManager.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <filesystem>
#include <fstream>

using namespace Smith;

std::map<std::string, std::shared_ptr<Texture>> Smith::ResourceManager::texture_cache;

void ResourceManager::Init()
{
    stbi_set_flip_vertically_on_load(true);
}

Texture* ResourceManager::GetTexture(const std::string& id)
{
    for (const auto& [key, value] : texture_cache)
        if (key == id)
            return value.get();
    Texture* tex = LoadTexture(id);
    if (tex != nullptr)
        return tex;
    else
        return GetMissingTexture();
}

Texture* ResourceManager::LoadTexture(const std::string& path)
{
    std::string absolute_path = std::filesystem::absolute(path).string(); 
    if (!std::filesystem::exists(absolute_path))
        return nullptr;

    std::ifstream file;
    file.open(path, std::ios::binary | std::ios::in);

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> img_buffer(size);
    file.read(reinterpret_cast<char*>(img_buffer.data()), size);
    int width, height, channel_count;
    unsigned char* loaded_image = stbi_load_from_memory(img_buffer.data(), size, &width, &height, &channel_count, 4);
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(width, height, channel_count, loaded_image);
    texture_cache[path] = texture;
    stbi_image_free(loaded_image);
    return texture.get();
}

Texture* ResourceManager::GetMissingTexture()
{
    if (texture_cache.contains("SRCSMITH_INTERNAL_MISSING"))
    {
        return GetTexture("SRCSMITH_INTERNAL_MISSING"); //just get the texture if we created it already
    }
    //Generate the texture
    unsigned char pixels[4 * 4 * 3];
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            int index = (y * 4 + x) * 3;
            bool isPurple = (x + y) % 2 == 0;
            pixels[index + 0] = isPurple ? 255 : 0;     // R
            pixels[index + 1] = 0;                      // G
            pixels[index + 2] = isPurple ? 255 : 0;     // B
        }
    }
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(4, 4, 3, pixels);
    texture->filter_type = TextureFilterType::FILTER_NEAREST;
    texture_cache["SRCSMITH_INTERNAL_MISSING"] = texture;
    return texture.get();
}