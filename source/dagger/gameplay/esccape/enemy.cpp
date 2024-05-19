#include "enemy.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

#include "gameplay/common/simple_collisions.h"

#include <algorithm>
#include <execution>
#include <chrono> // For std::chrono
#include <thread> // For std::this_thread

using namespace dagger;
using namespace esccape;

esccape::Enemy::Enemy()
{
}

esccape::Enemy::Enemy(EnemyEntity enemyEntity, std::function<void(int)> healthChangedCallback2) :
    enemy(enemyEntity), healthChangedCallback(healthChangedCallback2)
{
}

void Enemy::SpinUp()
{
}

void Enemy::WindDown()
{
}

int esccape::Enemy::getHealth()
{
    return enemy.health;
}

void esccape::Enemy::setHealth(int newHealth)
{
    enemy.health = newHealth;
}