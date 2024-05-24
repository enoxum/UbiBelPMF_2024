#include "core/engine.h"
#include "core/core.h"
#include "character_system.h"


using namespace dagger;
using namespace esccape;

void CharacterSystem::Run()
{
    auto& reg = Engine::Registry();
    BlackboardManager& bbManager = BlackboardManager::GetInstance();

    auto characterView = reg.view<Character>();
    for (auto entity : characterView)
    {
        auto& character = characterView.get<Character>(entity);
        character.CheckCollisions();
        //printf("*** %d *** %d ***\n", bbManager.HasCollided(entity), (int)entity);
        // Check if a collision has occurred using the BlackboardManager
        if (bbManager.HasCollided(entity))
        {
            // Resolve collision and perform additional actions if needed
            // Example: auto& fsmState = reg.get<CharacterControllerFSM::StateComponent>(entity);
            // ResolveCollision(character, collidedWith, bbManager, fsmState);
            printf("Collision detected for entity %d\n", (int)entity);
        }
    }
}           