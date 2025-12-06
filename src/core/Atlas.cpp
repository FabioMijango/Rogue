#include "Atlas.hpp"

Atlas::Atlas(const std::string &imagePath, const std::string &metaPath) {
    sf::Texture tempAtlas;
    if (!tempAtlas.loadFromFile(imagePath))
        throw std::runtime_error("Error abriendo " + imagePath);
    atlasTexture = std::make_shared<sf::Texture>(tempAtlas);

    parseMeta(metaPath);
}

sf::IntRect Atlas::getTileRect(int row, int col) {
    return sf::IntRect(
        { col * consts::TILE_SIZE, row * consts::TILE_SIZE},
        {consts::TILE_SIZE, consts::TILE_SIZE}
    );
}

sf::Sprite Atlas::getSprite(uint16_t id) const {
    const auto& tile = tiles.at(id);
    sf::Sprite sprite(*atlasTexture, getTileRect(tile.row, tile.col));
    return sprite;
}

void Atlas::parseMeta(const std::string &metaPath) {
    std::ifstream file(metaPath);
    if (!file.is_open()) {
        throw std::runtime_error("Error abriendo tiles.txt");
    }

    std::regex linePattern(R"((\d+)\.([a-z])\. (.*))");
    std::string line;

    uint16_t tileId = 0;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        if (std::smatch match; std::regex_match(line, match, linePattern)) {
            const uint16_t row = std::stoi(match[1]) - 1;
            const uint16_t colC = match[2].str()[0];
            const uint16_t col = colC - 'a';
            const std::string name = match[3];
            tiles[tileId++] = Tile{ row, col, name };
        }
    }
    file.close();
}
