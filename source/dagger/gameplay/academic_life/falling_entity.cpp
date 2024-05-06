#include "falling_entity.h"

#include "core/engine.h"
#include "core/game/transforms.h"

#include "gameplay/academic_life/academic_life_logic.h"

using namespace dagger;
using namespace academic_life;

void FallingEntitySystem::Run()
{
    AcademicLifeFieldSettings fieldSettings;
    if (auto* ptr = Engine::GetDefaultResource<AcademicLifeFieldSettings>())
    {
        fieldSettings = *ptr;
    }

    auto view = Engine::Registry().view<Transform, FallingEntity>();
    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& falling_entity = view.get<FallingEntity>(entity);

        t.position.y -= falling_entity.speed * Engine::DeltaTime();

        Float32 boarderY = fieldSettings.GetYBoarder();
        if (t.position.y < -boarderY)
        {
            t.position.y = boarderY;
            falling_entity.speed = fieldSettings.fieldTileSize * (rand() % 5 + 3);
        }
    }
}
