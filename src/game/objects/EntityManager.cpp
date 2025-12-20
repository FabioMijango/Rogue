#include "EntityManager.hpp"

sf::Sprite& EntityManager::getPlayerSprite() {
    return std::get<1>(playerSpritesCache);
}

sf::Sprite& EntityManager::getEnemySprite(uint16_t id) {
    return enemySpritesCache.at(id);
}
