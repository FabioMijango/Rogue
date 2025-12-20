#pragma once
#include <unordered_map>
#include <SFML/Graphics/Sprite.hpp>

class EntityManager {
public:
    sf::Sprite& getPlayerSprite();
    sf::Sprite& getEnemySprite(uint16_t id);
private:
    std::tuple<uint16_t, sf::Sprite> playerSpritesCache;
    std::unordered_map<uint16_t, sf::Sprite> enemySpritesCache;
    // std::unordered_map<uint16_t, sf::Sprite> itemSprites;
};
