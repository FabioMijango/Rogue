#include "ResourceManager.hpp"

ResourceManager::ResourceManager() {
    loadTextures();
}

void ResourceManager::loadTextures() {
    for (const auto& res : bb::resources) {
        atlasTextures[res.name] = std::make_shared<sf::Texture>(res.path);
    }
}

std::weak_ptr<sf::Texture> ResourceManager::getTexture(const std::string &textureName) {
    return atlasTextures[textureName];
}
