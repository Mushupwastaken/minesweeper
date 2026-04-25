#pragma once
#include <nlohmann/json_fwd.hpp>

namespace ne {

struct TextureAtlas {
private:
    sf::Texture m_texture;
    std::unordered_map<std::string, sf::IntRect> m_frames;
public:
    TextureAtlas(std::filesystem::path texturePath, std::filesystem::path jsonPath);

    const sf::Texture& getTexture() const;
    sf::IntRect getFrame(const std::string& name) const;
};

} //namespace ne