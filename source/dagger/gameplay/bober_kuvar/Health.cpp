/*#include "Health.h"


using namespace health_system;
using namespace bober_game;

void Health::Run()
{
    Float32 currentHealthRatio = 0.0f;
    {//calculate current player health ratio
        auto view = Engine::Registry().view<HealthComponent, ControllerMapping>();
        for (auto entity : view)
        {
            auto& healthComponent = view.get<HealthComponent>(entity);
            currentHealthRatio = healthComponent.currentHealth / (Float32)healthComponent.maxHealth;
        }
    }

    auto view = Engine::Registry().view<HealthBar>();
    for (auto entity : view)
    {
        auto& healthBar = view.get<HealthBar>(entity);

        auto& fillSprite = Engine::Registry().get<Sprite>(healthBar.fillEntity);

        Float32 fillWidth = std::clamp(currentHealthRatio, 0.f, 1.f) * healthBar.width; // calculate new width
        bool changed = fillSprite.size.x != fillWidth; //check if width actually changed
        Float32 oldWidth = fillSprite.size.x; //save oldwidth
        fillSprite.size = { fillWidth, healthBar.height };

        if (changed)
        {
            auto& fillSpriteTransform = Engine::Registry().get<Transform>(healthBar.fillEntity);
            fillSpriteTransform.position.x += (fillWidth - oldWidth) / 2;
        }

    }

}*/