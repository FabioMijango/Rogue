#include "ResourceManager.hpp"

#include "Atlas.hpp"

ResourceManager::ResourceManager() {
    loadTextures();
}

void ResourceManager::loadTextures() {
    for (const auto& entry : bb::resources) {
        const auto atlas = std::make_shared<Atlas>(entry.imagePath, entry.metaPath);
        atlasTextures[entry.name] = atlas;
    }
}

std::weak_ptr<Atlas> ResourceManager::getAtlas(const std::string &atlasName) {
    return atlasTextures[atlasName];
}
