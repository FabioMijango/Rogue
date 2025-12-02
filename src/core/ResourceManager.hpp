#pragma once

#include <string>
#include <unordered_map>

#include "SFML/Graphics/Texture.hpp"
#include "../bible/Bible.hpp"

class ResourceManager {
public:
    ResourceManager();

    std::weak_ptr<sf::Texture> getTexture(const std::string& textureName);

private:
    std::unordered_map<std::string, std::shared_ptr<sf::Texture> > atlasTextures;
private:
    void loadTextures();
};

struct Resource {

};