#include "core/engine.h"
#include "core/core.h"
#include "character_system.h"

#include "esccape_main.h"

#include <chrono>
#include <thread>

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
            ResolveCollision(entityA, entityB, bbManager);
        }

        auto& chr = Engine::Registry().get<EsccapeCharacter>(entity);

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
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        
        if(m_winnerID == 1)
            AssignSprite(sprite, "Esccape:game_over1");
        if (m_winnerID == 0)
            AssignSprite(sprite, "Esccape:game_over0");

    }
}
