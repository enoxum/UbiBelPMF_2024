#include "score_entity.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "health.h"

using namespace dagger;
using namespace academic_life;


void ScoreEntitySystem::Run()
{
    auto& registry = Engine::Registry();
    auto view = registry.view<ScoreEntityBase, Transform>(); // Pogled na entitete sa ScoreEntityBase i Transform komponentama
    Health& health = Health::Instance();

    for (auto entity : view)
    {
        auto& scoreEntity = registry.get<ScoreEntityBase>(entity);
        auto& transform = registry.get<Transform>(entity);
        scoreEntity.value = health.GetValue();

        scoreEntity.SetText("pixel-font", std::to_string(scoreEntity.value), transform.position);
    }
    
}