#include "Atlas.hpp"

#include <iostream>

Atlas::Atlas(const std::string &imagePath, const std::string &metaPath) {
    sf::Texture tempAtlas;
    if (!tempAtlas.loadFromFile(imagePath))
        throw std::runtime_error("Error abriendo " + imagePath);
    atlasTexture = std::make_shared<sf::Texture>(tempAtlas);

    parseMeta(metaPath);
}

sf::IntRect Atlas::getTileRect(int row, int col) {
    return sf::IntRect(
        {row, col},
        {32, 32});
}

sf::Sprite Atlas::getSprite(const std::string &tileName) const {
    auto [row, col] = tiles.at(tileName);

    sf::Sprite sprite(*atlasTexture, getTileRect(row, col));
    return sprite;
}

void Atlas::parseMeta(const std::string &metaPath) {
    std::ifstream file(metaPath);
    if (!file.is_open()) {
        throw std::runtime_error("Error abriendo tiles.txt");
    }

    std::regex linePattern(R"((\d+)\.([a-z])\. (.*))");
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) {
            std::cout << "Empty line" << std::endl;
            continue;
        }

        if (std::smatch match; std::regex_match(line, match, linePattern)) {
            const int row = std::stoi(match[1]);
            const int colC = match[2].str()[0];
            const int col = colC - 'a';
            std::cout << row << " " << col << " " << match[3] << std::endl;
            std::string name = match[3];

            tiles[name] = Tile{(row - 1) * 32, col * 32};
        }
    }
}
