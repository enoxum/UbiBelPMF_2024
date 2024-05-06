#include "academic_life_logic.h"

#include "core/engine.h"
#include "core/game/transforms.h"

#include "gameplay/common/simple_collisions.h"
#include "gameplay/academic_life/academic_player.h"
#include "gameplay/academic_life/academic_life_main.h"

using namespace dagger;
using namespace academic_life;

void AcademicLifeCollisionsLogicSystem::SpinUp()
{
    Engine::Dispatcher().sink<NextFrame>().connect<&AcademicLifeCollisionsLogicSystem::OnEndOfFrame>(this);
}

void AcademicLifeCollisionsLogicSystem::WindDown()
{
    Engine::Dispatcher().sink<NextFrame>().disconnect<&AcademicLifeCollisionsLogicSystem::OnEndOfFrame>(this);
}

void AcademicLifeCollisionsLogicSystem::Run()
{
    AcademicLifeFieldSettings fieldSettings;
    {
        static Entity entityFieldSettings;
        if (auto* ptr = Engine::GetDefaultResource<AcademicLifeFieldSettings>())
        {
            fieldSettings = *ptr;
        }

        auto view = Engine::Registry().view<AcademicPlayer, Transform, SimpleCollision>();
        for (auto entity : view)
        {
            auto &t = view.get<Transform>(entity);
            auto &player = view.get<AcademicPlayer>(entity);
            auto &col = view.get<SimpleCollision>(entity);

            if (col.colided)
            {
                m_Restart = true;
            }
        }
    }
}

void AcademicLifeCollisionsLogicSystem::OnEndOfFrame()
{
    if (m_Restart)
    {
        m_Restart = false;
        Engine::Registry().clear();

        academic_life::SetupWorld();
    }
}