#pragma once

#include "core/graphics/text.h"
#include "core/system.h"
#include "core/core.h"

#include "gameplay/academic_life/academic_life_logic.h"

using namespace dagger;

namespace academic_life
{
    struct FallingEntityBase
    {
        Float32 speed;
        virtual void SetText(const String& font, const String& text, const Vector3& position) = 0;
    };

    struct FallingEntity : public FallingEntityBase
    {
        void SetText(const String& font, const String& text, const Vector3& position) override
        {   }
    };

    struct FallingText : public FallingEntityBase
    {
        Text text;
        void SetText(const String& font, const String& text, const Vector3& position) override
        {
            this->text.Set(font, text, position);
        }
    };

    template<typename T>
    void ProcessEntity(Float32 boarderY, float mul, float entitySpeed, int& fallingEntityCounter,
        AcademicLifeFieldSettings fieldSettings, bool isText)
    {
        auto view = Engine::Registry().view<Transform, T>();
        for (auto entity : view)
        {
            auto& transform = view.get<Transform>(entity);
            auto& falling_entity = view.get<T>(entity);
            falling_entity.speed = entitySpeed;

            transform.position.y -= falling_entity.speed * Engine::DeltaTime();

            // Umesto da se vrati gore kada padne na zemlju, bolje je da se kreira novi objekat (drugog tipa)
            if (transform.position.y < -boarderY)
            {
                transform.position.y = boarderY;
                fallingEntityCounter += 1;
                if (fallingEntityCounter % 3 == 0)
                {
                    mul += 0.4;
                    entitySpeed = fieldSettings.fieldTileSize * mul;
                }
            }

            if (isText)
                falling_entity.SetText("pixel-font", "jednacina", transform.position);
        }
    }

    class FallingEntitySystem : public System
    {
    public:
        inline String SystemName() { return "Falling Entity's System"; }

        void Run() override;
    };
}
