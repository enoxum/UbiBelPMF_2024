#include "game_state.h"
#include "core/graphics/text.h"
#include "core/system.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"
#include "core/game/transforms.h"
#include "gameplay/academic_life/academic_life_logic.h"
#include "gameplay/academic_life/academic_life_main.h"

using namespace academic_life;

void GameStateInputSystem::Run()
{
    Engine::Registry().view<GameStateControllerMapping>().each([&](GameStateControllerMapping& ctrl_)
        {
            if (ctrl_.input.x == 1)
            {
                Engine::Registry().clear();
                academic_life::SetupMainMenu();
                ctrl_.input.x = 0;
            }
        });
}


void GameStateInputSystem::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&GameStateInputSystem::OnKeyboardEvent>(this);
}

void GameStateInputSystem::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&GameStateInputSystem::OnKeyboardEvent>(this);
}

void GameStateInputSystem::OnKeyboardEvent(KeyboardEvent kEvent_)
{
    Engine::Registry().view<GameStateControllerMapping>().each([&](GameStateControllerMapping& ctrl_)
        {
            if (kEvent_.key == ctrl_.spaceKey && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
            {
                ctrl_.input.x = 1;
            }
        });
}

void SetupScreen(const std::string& message)
{
    auto& engine = Engine::Instance();
    auto& reg = engine.Registry();

    auto entity = reg.create();
    academic_life::CreateBackdrop();
    reg.emplace<GameStateControllerMapping>(entity);

    auto messageEntity = reg.create();
    auto& messageText = reg.emplace<Text>(messageEntity);
    messageText.scale = Vector2{ 2.5f, 2.5f };
    messageText.spacing = 2.5f;
    messageText.Set("pixel-font", message, glm::vec3(20.f / 2.0f, 100.0f / 2.0f + 20.f / 2.0f, 0.5f), 0.5f + 0.1f);

    auto instructionsEntity = reg.create();
    auto& instructionsText = reg.emplace<Text>(instructionsEntity);
    instructionsText.spacing = 0.6f;
    instructionsText.Set("pixel-font", "Press Space to go back to the main menu", glm::vec3(20.f / 2.0f, -300.0f / 2.0f + 20.f / 2.0f, 0.5f), 0.05f);
}

void academic_life::GameOverScreen()
{
    SetupScreen("GAME OVER");
}

void academic_life::WinScreen()
{
    SetupScreen("You win :)");
}