#include "academic_life_logic.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/text.h"


#include "gameplay/common/simple_collisions.h"
#include "academic_player.h"
#include "academic_life_main.h"
#include "falling_entity.h"
#include "game_state.h"

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
                int score = 11;
                if (score < 0) {
                    m_Restart = true;
                }

                auto viewEntities = Engine::Registry().view<FallingEntity,LifestyleChange>();
                for (auto entityEntity : viewEntities)
                {
                    auto lifestyleChange = static_cast<int>(Engine::Registry().get<LifestyleChange>(entityEntity));
                    if (entityEntity == col.colidedWith)
                    {
                        ESPB& espb = ESPB::Instance();
                        Health& health = Health::Instance();
                        health.Update(lifestyleChange);
                          if (health.GetValue() <= -100) {

                            Engine::Registry().clear();
                            academic_life::GameOverScreen();
                            health.Reset();
                            espb.Reset();
                            return;

                        }
                        Engine::Registry().destroy(entityEntity);  // delete current entity
                        createRandomEntity();  // create new random entity
                        break;
                    }
                }

                auto viewEntities2 = Engine::Registry().view<FallingText>();
                for (auto entityEntity : viewEntities2)
                {
                    if (entityEntity == col.colidedWith)
                    {
                        auto& falling_text = viewEntities2.get<FallingText>(entityEntity);
                        falling_text.text.Set("pixel-font", "", falling_text.text.position);

                        Health& health = Health::Instance();
                        ESPB& espb = ESPB::Instance();
                        espb.Update(falling_text.text.value);
                        if (espb.GetValue() == 240) {

                            ScoreEntitySystem::updateBestTime();
                            Engine::Registry().clear();
                            academic_life::WinScreen();
                            espb.Reset();
                            health.Reset();
                            return;

                        }

                        if (espb.GetValue() < 0) {

                            Engine::Registry().clear();
                            academic_life::GameOverScreen();
                            espb.Reset();
                            health.Reset();
                            return;

                        }
                        std::cout << falling_text.text.value << std::endl;
               
                        Engine::Registry().destroy(entityEntity);  // delete current entity
                        createRandomEntity();  // create new random entity
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