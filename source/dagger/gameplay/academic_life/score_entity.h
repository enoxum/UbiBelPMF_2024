#pragma once

#include "core/graphics/text.h"
#include "core/system.h"
#include "core/core.h"

#include "gameplay/academic_life/academic_life_logic.h"
#include "core/game/transforms.h"


using namespace dagger;

namespace academic_life
{
    struct ScoreEntityBase
    {
        int value;
        //Text text;
        //void SetText(const String& font, const String& text, const Vector3& position);
    };

    //void ScoreEntityBase::SetText(const String& font, const String& text, const Vector3& position)
    //{
    //    this->text.Set(font, text, position);
    //}
    
    class ScoreEntitySystem : public System
    {
    public:
        inline String SystemName() { return "Score Entity's System"; }

        void Run() override;        

        //void SetTextFromValue(int value)
        //{
        //    auto view = Engine::Registry().view<ScoreEntityBase>(); // pogled na entitete sa ScoreEntityBase komponentom
        //    for (auto entity : view)
        //    {
        //        auto& scoreEntityBase = view.get<ScoreEntityBase>(entity); // referenca na ScoreEntityBase komponentu
        //        auto& transform = view.get<Transform>(entity); // Dobijanje transformacije entiteta

        //        scoreEntityBase.SetText("pixel-font", std::to_string(scoreEntityBase.value), Vector3{ 0, 0, 0 }); // Postavite tekst na vrednost value
        //    }
        //}
    };
}
