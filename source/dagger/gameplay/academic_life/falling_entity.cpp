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

    Float32 boarderY = fieldSettings.GetYBoarder();
    static float mul = 4;
    static int fallingEntityCounter = 0;
    float entitySpeed = fieldSettings.fieldTileSize * mul;

    auto view = Engine::Registry().view<Transform, FallingEntity>();
    for (auto entity : view)
    {
        auto& transform = view.get<Transform>(entity);
        auto& falling_entity = view.get<FallingEntity>(entity);
        falling_entity.speed = entitySpeed;

        transform.position.y -= falling_entity.speed * Engine::DeltaTime();

        // Umesto da se vrati gore kada padne na zemlju, bolje je da se kreira novi objekat (drugog tipa)
        if (transform.position.y < -boarderY)
        {
            transform.position.y = boarderY;
            fallingEntityCounter += 1;
            if (fallingEntityCounter % 3 == 0) {
                mul += 0.4;
                entitySpeed = fieldSettings.fieldTileSize * mul;
            }
        }
    }

    mul = 4;
    fallingEntityCounter = 0;
    entitySpeed = fieldSettings.fieldTileSize * mul;
    auto view2 = Engine::Registry().view<Transform, FallingText>();
    for (auto entity : view2)
    {
        auto& transform = view2.get<Transform>(entity);
        auto& falling_entity = view2.get<FallingText>(entity);
        falling_entity.speed = entitySpeed;
        auto& text = falling_entity.text;

        transform.position.y -= falling_entity.speed * Engine::DeltaTime();

        // Umesto da se vrati gore kada padne na zemlju, bolje je da se kreira novi objekat (drugog tipa)
        if (transform.position.y < -boarderY)
        {
            transform.position.y = boarderY;
            fallingEntityCounter += 1;
            if (fallingEntityCounter % 3 == 0) {
                mul += 0.4;
                entitySpeed = fieldSettings.fieldTileSize * mul;
            }
        }

        text.Set("pixel-font", "jednacina", transform.position);
    }
}