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
        Text text;
        void SetText(const String& font, const String& text, const Vector3& position) {
            this->text.Set(font, text, position);
        }
    };

    
    class ScoreEntitySystem : public System
    {
    public:
        inline String SystemName() { return "Score Entity's System"; }

        void Run() override;        

    };
}
