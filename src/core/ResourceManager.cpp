#include "ResourceManager.hpp"

ResourceManager::ResourceManager() {
    loadTextures();
    loadFonts();
}

void ResourceManager::loadTextures() {
    for (const auto& entry : bb::imageResources) {
        const auto atlas = std::make_shared<Atlas>(entry.path, entry.metaPath);
        atlasTextures[entry.name] = atlas;
    }
}

void ResourceManager::loadFonts() {
    for (const auto& entry : bb::fontResources) {
        const auto font = std::make_shared<sf::Font>(entry.path);
        fonts[entry.name] = font;
    }
}

std::weak_ptr<Atlas> ResourceManager::getAtlas(const std::string &atlasName) {
    return atlasTextures[atlasName];
}

std::weak_ptr<sf::Font> ResourceManager::getFont(const std::string &fontName) {
    return fonts[fontName];
}
