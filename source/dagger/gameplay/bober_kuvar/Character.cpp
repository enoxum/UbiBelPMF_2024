/*#include "Character.h"

using namespace bober_game

Character::Character(double hp, double speed, double strength, const std::string& sprite_path, const std::string& animation_path, bool collidable, std::pair<int, int> collision_size)
	: OurEntity(sprite_path, animation_path, collidable, collision_size), hp_(hp), speed_(speed), strength_(strength)
{
	collidable_ = collidable;
	collision_size_ = collision_size;
    createHealth();
}

void Character::spawn(const std::pair<int, int>&, const std::pair<int, int>&, const std::vector<std::vector<int>>&)
{
}

void Character::die()
{
}

void Character::collision()
{
}

void Character::createHealth()
{
    auto& engine = Engine::Instance();
    auto& reg = engine.Registry();


    healthBarEntity = reg.create();
    auto& healthBar = reg.emplace<HealthBar>(healthBarEntity);
    healthBar.width = 100;
    healthBar.height = 15;
    healthBar.borderThickness = 5; 
    healthBar.backColor = { 0.f, 0.f, 0.f, 1.f };
    healthBar.fillColor = { 0.1f, 0.9f, 0.1f, 1.f };

    auto backgroundEntity = reg.create();
    auto& bgSprite = reg.get_or_emplace<Sprite>(backgroundEntity);
    auto& bgTransform = reg.get_or_emplace<Transform>(backgroundEntity);
    bgTransform.position = this->transform->position;

    AssignSprite(bgSprite, "EmptyWhitePixel");
    bgSprite.size = { healthBar.width + healthBar.borderThickness, healthBar.height + healthBar.borderThickness };
    bgSprite.color = healthBar.backColor;

    auto fillEntity = reg.create();
    auto& fillSprite = reg.get_or_emplace<Sprite>(fillEntity);
    auto& fillTransform = reg.get_or_emplace<Transform>(fillEntity);
    fillTransform.position = this->transform->position;

    AssignSprite(fillSprite, "EmptyWhitePixel");
    fillSprite.size = { healthBar.width, healthBar.height };
    fillSprite.color = healthBar.fillColor;

    healthBar.backgroundEntity = backgroundEntity;
    healthBar.fillEntity = fillEntity;


    health = reg.emplace<HealthComponent>(this->instance);
    health.maxHealth = 100;
    health.currentHealth = 100;

}

double Character::getSpeed() {
	return speed_;
}
*/
