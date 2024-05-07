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
    static int fallingEntityCounter = 0;
    static float mul = 4;
    float entitySpeed = fieldSettings.fieldTileSize * mul;

    auto view = Engine::Registry().view<Transform, FallingEntity>();
    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& falling_entity = view.get<FallingEntity>(entity);
        falling_entity.speed = entitySpeed;

        t.position.y -= falling_entity.speed * Engine::DeltaTime();
        

        Float32 boarderY = fieldSettings.GetYBoarder();
        if (t.position.y < -boarderY)
        {
            t.position.y = boarderY;
            fallingEntityCounter += 1;
            if (fallingEntityCounter % 3 == 0) {
                mul += 0.4;
                entitySpeed = fieldSettings.fieldTileSize * mul;
            }


            

        }
    }
}
