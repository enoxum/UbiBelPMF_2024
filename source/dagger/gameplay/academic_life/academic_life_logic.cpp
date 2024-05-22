#include "academic_life_logic.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/text.h"

#include "gameplay/common/simple_collisions.h"
#include "academic_player.h"
#include "academic_life_main.h"
#include "falling_entity.h"

#include "health.h"
#include "espb.h"
#include "score_entity.h"
#include "enumi.h"

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
                // TO DO: score logic
                int score = 11;
                if (score < 0) {
                    m_Restart = true;
                }

                // TO DO: create random entity
                auto viewEntities = Engine::Registry().view<FallingEntity,LifestyleChange>();
                for (auto entityEntity : viewEntities)
                {
                    auto lifestyleChange = static_cast<int>(Engine::Registry().get<LifestyleChange>(entityEntity));

                    if (entityEntity == col.colidedWith)
                    {
                        Health& health = Health::Instance();
                        health.Increase(lifestyleChange);             //TO DO Decrease proteklim vremenom 
                        Engine::Registry().destroy(entityEntity);
                        break;
                    }
                }

                // TO DO: create random entity
                auto viewEntities2 = Engine::Registry().view<FallingText>();
                for (auto entityEntity : viewEntities2)
                {
                    if (entityEntity == col.colidedWith)
                    {
                        auto& falling_text = viewEntities2.get<FallingText>(entityEntity);
                        falling_text.text.Set("pixel-font", "", falling_text.text.position);

                        ESPB& espb = ESPB::Instance();
                        espb.Increase(falling_text.text.value);
                        std::cout << falling_text.text.value << std::endl;
               
                        Engine::Registry().destroy(entityEntity);
                        break;
                    }
                }
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