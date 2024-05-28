#include "core/engine.h"
#include "core/core.h"
#include "character_system.h"

#include "esccape_main.h"

using namespace dagger;
using namespace esccape;

void CharacterSystem::SpinUp()
{
    Engine::Dispatcher().sink<NextFrame>().connect<&CharacterSystem::OnEndOfFrame>(this);
}

void CharacterSystem::WindDown()
{
    Engine::Dispatcher().sink<NextFrame>().disconnect<&CharacterSystem::OnEndOfFrame>(this);
}

void CharacterSystem::Run()
{
    auto& reg = Engine::Registry();
    BlackboardManager& bbManager = BlackboardManager::GetInstance();

    auto characterView = reg.view<Character>();
    for (auto entity : characterView)
    {
        auto& character = characterView.get<Character>(entity);
        auto collidedPair = character.CheckCollisions();  

        Entity entityA = collidedPair.first;
        Entity entityB = collidedPair.second;

        if (entityA != entt::null && entityB != entt::null)
        {
            //printf("Collision detected between entity %d and entity %d\n", static_cast<int>(entityA), static_cast<int>(entityB));
            ResolveCollision(entityA, entityB, bbManager);
        }

        auto& chr = Engine::Registry().get<EsccapeCharacter>(entity);

        //printf("HEALTH: %f    IS_ALIVE: %d \n", chr.healthSystem.GetCurrentHealth(), chr.healthSystem.IsAlive());
        if (chr.healthSystem.IsAlive() == false) {
            m_Restart = true;
            m_winnerID = (chr.id == 1) ? 0 : 1;
        }
    }
}

void esccape::CharacterSystem::OnEndOfFrame()
{
    if (m_Restart)
    {
        printf("END OF GAME\n");
        m_Restart = false;

        Engine::Instance().Registry().clear();
        Engine::Dispatcher().clear();

        auto ui1 = Engine::Registry().create();
        auto& sprite = Engine::Registry().get_or_emplace<Sprite>(ui1);
        sprite.size = { 800, 600};
        sprite.position = { 0, 0, 0 };
        
        if(m_winnerID == 1)
            AssignSprite(sprite, "Esccape:game_over1");
        if (m_winnerID == 0)
            AssignSprite(sprite, "Esccape:game_over0");


        // FIX: ne radi restartovanjeee :(((
        //esccape::SetupWorld();
    }
}
