#pragma once
#include <fstream>
#include <regex>
#include <string>
#include <unordered_map>

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"

struct Tile {
    int row;
    int col;
};

class Atlas {
public:
    Atlas(const std::string& imagePath, const std::string& metaPath);
    sf::Sprite getSprite(const std::string& tileName) const;

private:
    std::shared_ptr<sf::Texture> atlasTexture;
    std::unordered_map<std::string, Tile> tiles;
private:
    static sf::IntRect getTileRect(int row, int col);
    void parseMeta(const std::string& metaPath);
};
