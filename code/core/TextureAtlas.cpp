#include "TextureAtlas.hpp"
#include <fstream>

namespace ne {

TextureAtlas::TextureAtlas(std::filesystem::path texturePath, std::filesystem::path jsonPath) {
    //Open Texture (flag any errors)
    if(!m_texture.loadFromFile(texturePath))
    {
        throw sf::Exception("Failed to load texture");
    }
    
    //Open JSON (flag any errors)
    std::ifstream file(jsonPath);
    if(!file.is_open())
    {
        throw sf::Exception("Failed to load json");
    }

    auto context = json::parse(file);
    file.close();
    
    //Add to umap
    for(const auto& [key, value] : context["frames"].items()) 
    {
        auto& frame = value["frame"];

        int x = frame.value("x", 0);
        int y = frame.value("y", 0);
        int w = frame.value("w", 0);
        int h = frame.value("h", 0);

        std::filesystem::path name(key);
        name.replace_extension("");

        m_frames[name.string()] = sf::IntRect{{x, y}, {w, h}};
    }
}

const sf::Texture& TextureAtlas::getTexture() const {
    return m_texture;
}

sf::IntRect TextureAtlas::getFrame(const std::string& name) const {
    if(auto it = m_frames.find(name); it != m_frames.end()) 
    {
        return it->second;
    }

    return sf::IntRect{};
}

} //namespace ne