#include "score_entity.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "health.h"
#include "espb.h"

using namespace dagger;
using namespace academic_life;

namespace academic_life {
    time_t ScoreEntitySystem::bestTime = 0;
} 



void ScoreEntitySystem::Run()
{   
    updateTime();
    auto& registry = Engine::Registry();
    auto view = registry.view<ScoreEntityBase, Transform, int>(); // TO DO switch int to EnumiScore
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

void academic_life::ScoreEntitySystem::SpinUp()
{
}

void academic_life::ScoreEntitySystem::WindDown()
{
}

void academic_life::ScoreEntitySystem::updateTime() {
    auto view = Engine::Registry().view<TimeComponent, Text>();
    
    if (!view.empty()) {
        auto entity = view.front();
        auto& text = view.get<Text>(entity);
        auto& time = view.get<TimeComponent>(entity);
        text.Set("pixel-font", std::to_string(std::time(0) - time.beginTime), { 340.0f, 220.0f, 0.5f });
    }
}


time_t academic_life::ScoreEntitySystem::getBestTime() {
    return bestTime;
}

void academic_life::ScoreEntitySystem::updateBestTime() {
    auto view = Engine::Registry().view<TimeComponent, Text>();
    auto entity = view.front();
    auto endTime = std::time(0) - view.get<TimeComponent>(entity).beginTime;
    
    if (bestTime == 0 || endTime < bestTime)
        bestTime = endTime;
}
