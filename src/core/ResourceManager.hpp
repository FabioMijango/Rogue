#pragma once

#include <string>
#include <unordered_map>

#include "Atlas.hpp"
#include "../bible/Bible.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Texture.hpp"

class ResourceManager {
public:
    ResourceManager();
    std::weak_ptr<Atlas> getAtlas(const std::string& atlasName);
    std::weak_ptr<sf::Font> getFont(const std::string& fontName);
private:
    std::unordered_map<std::string, std::shared_ptr<Atlas>> atlasTextures;
    std::unordered_map<std::string, std::shared_ptr<sf::Font>> fonts;
private:
    void loadTextures();
    void loadFonts();
};

struct Resource {

};