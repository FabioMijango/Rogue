#pragma once

#include <string>
#include <unordered_map>

#include "Atlas.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "../bible/Bible.hpp"

class ResourceManager {
public:
    ResourceManager();
    std::weak_ptr<Atlas> getAtlas(const std::string& atlasName);

private:
    std::unordered_map<std::string, std::shared_ptr<Atlas>> atlasTextures;
private:
    void loadTextures();
};

struct Resource {

};