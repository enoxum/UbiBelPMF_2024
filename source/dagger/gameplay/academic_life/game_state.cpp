#include "game_state.h"
#include "core/graphics/text.h"
#include "core/system.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"
#include "core/game/transforms.h"
#include "gameplay/academic_life/academic_life_logic.h"
#include "gameplay/academic_life/academic_life_main.h"
#include <iostream>
#include "gameplay/academic_life/score_entity.h"
#include <climits>

using namespace academic_life;
namespace academic_life {
    Sprite* arrowSprite = nullptr;
}
void GameStateInputSystem::Run() {

    auto& reg = Engine::Registry();

    // EndScreen
    reg.view<GameStateControllerMapping, EndScreenState>().each([&](GameStateControllerMapping& ctrl_, EndScreenState& state) {
        if (ctrl_.input.x == 1) { // If Right key (D) is pressed
            state.selectedOption = Option::No;
            ctrl_.input.x = 0;
            arrowSprite->position = glm::vec3(68.f, -250.0f / 2.0f + 20.f / 2.0f, 0.5f);
        }
        else if (ctrl_.input.y == 1) { // If Left key (A) is pressed
            state.selectedOption = Option::Yes;
            ctrl_.input.y = 0;
            arrowSprite->position = glm::vec3(-75.f, -250.0f / 2.0f + 20.f / 2.0f, 0.5f);
        }

        if (ctrl_.input.z == 1) { // If Space key is pressed
            if (state.selectedOption == Option::Yes) {
                reg.clear();
                academic_life::SetupWorld();
            }
            else if (state.selectedOption == Option::No) {
                reg.clear();
                academic_life::SetupMainMenu();
            }
            ctrl_.input.z = 0;
        }
        });
    // WinScreen
    reg.view<GameStateControllerMapping>().each([&](GameStateControllerMapping& ctrl_) {
        if (ctrl_.input.z == 1) {
            reg.clear();
            academic_life::SetupMainMenu();
            ctrl_.input.z = 0;
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

void GameStateInputSystem::OnKeyboardEvent(KeyboardEvent kEvent_) {
    auto& reg = Engine::Registry();
    reg.view<GameStateControllerMapping>().each([&](GameStateControllerMapping& ctrl_) {
        if (kEvent_.key == ctrl_.spaceKey && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held)) {
            ctrl_.input.z = 1;
        }
        else if (kEvent_.key == ctrl_.leftKey && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held)) {
            ctrl_.input.y = 1;
        }
        else if (kEvent_.key == ctrl_.rightKey && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held)) {
            ctrl_.input.x = 1;
        }

        });
}

void academic_life::GameOverScreen()
{

    auto& engine = Engine::Instance();
    auto& reg = engine.Registry();

    auto entity = reg.create();
    academic_life::CreateBackdrop();
    reg.emplace<GameStateControllerMapping>(entity);
    reg.emplace<EndScreenState>(entity);

    //arrow
    auto arrowEntity = reg.create();
    arrowSprite = &reg.emplace<Sprite>(arrowEntity);
    AssignSprite(*(arrowSprite), "AcademicLife:pointerarrow");
    arrowSprite->size = { 25.0f, 25.0f };
    arrowSprite->position = { -75.f, -250.0f / 2.0f + 20.f / 2.0f, 0.5f };

    auto messageEntity = reg.create();
    auto& messageText = reg.emplace<Text>(messageEntity);
    messageText.scale = Vector2{ 2.5f, 2.5f };
    messageText.spacing = 2.5f;
    messageText.Set("pixel-font", "GAME OVER", glm::vec3(50.f, 100.0f / 2.0f + 20.f / 2.0f, 0.5f), 0.5f);

    auto instructionsEntity = reg.create();
    auto& instructionsText = reg.emplace<Text>(instructionsEntity);
    instructionsText.spacing = 0.6f;
    instructionsText.Set("pixel-font", "Press Space to select", glm::vec3(20.f, -350.0f / 2.0f + 20.f / 2.0f, 0.5f), 0.05f);

    auto playAgainEntity = reg.create();
    auto& playAgainText = reg.emplace<Text>(playAgainEntity);
    playAgainText.Set("pixel-font", "Play Again?", glm::vec3(25.f, -120.0f / 2.0f + 20.f / 2.0f, 0.5f), 0.1f);

    auto yesEntity = reg.create();
    auto& yesText = reg.emplace<Text>(yesEntity);
    yesText.Set("pixel-font", "Yes", glm::vec3(-50.f, -200.0f / 2.0f + 20.f / 2.0f, 0.5f), 0.1f);

    auto noEntity = reg.create();
    auto& noText = reg.emplace<Text>(noEntity);
    noText.Set("pixel-font", "No", glm::vec3(90.f, -200.0f / 2.0f + 20.f / 2.0f, 0.5f), 0.1f);

}

void academic_life::WinScreen()
{
    auto& engine = Engine::Instance();
    auto& reg = engine.Registry();

    auto entity = reg.create();
    academic_life::CreateBackdrop();
    reg.emplace<GameStateControllerMapping>(entity);

    auto messageEntity = reg.create();
    auto& messageText = reg.emplace<Text>(messageEntity);
    messageText.scale = Vector2{ 2.5f, 2.5f };
    messageText.spacing = 2.2f;
    messageText.Set("pixel-font", "You win : )", glm::vec3(50.f, 100.0f / 2.0f + 20.f / 2.0f, 0.5f), 0.5f + 0.1f);

    auto instructionsEntity = reg.create();
    auto& instructionsText = reg.emplace<Text>(instructionsEntity);
    instructionsText.spacing = 0.6f;
    instructionsText.Set("pixel-font", "Press Space to go back to the main menu", glm::vec3(20.f / 2.0f, -350.0f / 2.0f + 20.f / 2.0f, 0.5f), 0.05f);
    
    auto bestScoreEntity = reg.create();
    auto& bestScoreText = reg.emplace<Text>(bestScoreEntity);
    bestScoreText.Set("pixel-font", "Best time: " + std::to_string(ScoreEntitySystem::getBestTime()), glm::vec3(0.0f, -230.0f, 0.5f), 0.1f);
    }