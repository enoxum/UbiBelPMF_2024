#include "score_entity.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "health.h"
#include "espb.h"

using namespace dagger;
using namespace academic_life;


void ScoreEntitySystem::Run()
{
    auto& registry = Engine::Registry();
    auto view = registry.view<ScoreEntityBase, Transform, int>(); // Pogled na entitete sa ScoreEntityBase i Transform komponentama
    Health& health = Health::Instance();
    ESPB& espb = ESPB::Instance();

    for (auto entity : view)
    {
        auto& scoreEntity = registry.get<ScoreEntityBase>(entity);
        auto& transform = registry.get<Transform>(entity);
        auto marker = registry.get<int>(entity);

        if (marker == 1) {
            scoreEntity.value = health.GetValue();
        }
        else {
            scoreEntity.value = espb.GetValue();
        }


        scoreEntity.SetText("pixel-font", std::to_string(scoreEntity.value), transform.position);
    }
    
}