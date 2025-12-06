#pragma once
#include <fstream>
#include <regex>
#include <string>
#include <unordered_map>

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "../util/Constants.hpp"

struct Tile {
    uint16_t row;
    uint16_t col;
    std::string name;
};

class Atlas {
public:
    Atlas(const std::string& imagePath, const std::string& metaPath);
    sf::Sprite getSprite(uint16_t id) const;

private:
    std::shared_ptr<sf::Texture> atlasTexture;
    std::unordered_map<uint16_t, Tile> tiles;
private:
    static sf::IntRect getTileRect(int row, int col);
    void parseMeta(const std::string& metaPath);
};