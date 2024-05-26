#include "pingpong_playerinput.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include <imgui.h>
#include <tools/toolmenu.h>

using namespace dagger;
using namespace pacman;

Float32 PingPongPlayerInputSystem::s_BoarderDown = -20;
Float32 PingPongPlayerInputSystem::s_BoarderUp = 20;

Float32 PingPongPlayerInputSystem::s_PlayerSpeed = 1.f;

void PingPongPlayerInputSystem::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&PingPongPlayerInputSystem::OnKeyboardEvent>(this);
    Engine::Dispatcher().sink<ToolMenuRender>().connect<&PingPongPlayerInputSystem::RenderToolMenu>(this);

    Engine::Dispatcher().sink<GUIRender>().connect<&PingPongPlayerInputSystem::RenderToolWindow>(this);
}

void PingPongPlayerInputSystem::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&PingPongPlayerInputSystem::OnKeyboardEvent>(this);
    Engine::Dispatcher().sink<ToolMenuRender>().disconnect<&PingPongPlayerInputSystem::RenderToolMenu>(this);

    Engine::Dispatcher().sink<GUIRender>().disconnect<&PingPongPlayerInputSystem::RenderToolWindow>(this);
}

void PingPongPlayerInputSystem::OnKeyboardEvent(KeyboardEvent kEvent_)
{
    Engine::Registry().view<ControllerMapping>().each([&](ControllerMapping& ctrl_)
    {
        //Y
        if (kEvent_.key == ctrl_.up_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
        {
            ctrl_.input.y = 1;
        }
        else if (kEvent_.key == ctrl_.up_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.y > 0)
        {
            ctrl_.input.y = 1;
        }
        else if (kEvent_.key == ctrl_.down_key && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
        {
            ctrl_.input.y = -1;
        }
        else if (kEvent_.key == ctrl_.down_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.y < 0)
        {
            ctrl_.input.y =-1;
        }

        //X
        if (kEvent_.key == ctrl_.right_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
        {
            ctrl_.input.x = 1;
        }
        else if (kEvent_.key == ctrl_.right_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x > 0)
        {
            ctrl_.input.x = 1;
        }
        else if (kEvent_.key == ctrl_.left_key && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
        {
            ctrl_.input.x = -1;
        }
        else if (kEvent_.key == ctrl_.left_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x < 0)
        {
            ctrl_.input.x = -1;
        }
    });
}


#if !defined(NDEBUG)
void PingPongPlayerInputSystem::RenderToolMenu()
{
    if (ImGui::BeginMenu("Nas test"))
    {
        if (ImGui::MenuItem("Pause"))
        {
            dagger::Engine::ToggleSystemsPause(true);
        }
        ImGui::EndMenu();
    }
}

void PingPongPlayerInputSystem::RenderToolWindow()
{
    ImGui::Begin("Nas prozor");

    int i = 0;

    auto view = Engine::Registry().view<Transform>();
    for (auto entity : view)
    {
        ImGui::PushID(i);

        auto& t = view.get<Transform>(entity);

        int pos[]{ t.position.x, t.position.y };
        ImGui::SliderInt2("Pixel Size", pos, -10000.0f, 10000.0f, "%f", 1);

        t.position.x = pos[0];
        t.position.y = pos[1];

        i++;
        ImGui::PopID();
    }

    ImGui::End();
}

#endif //!defined(NDEBUG)

void PingPongPlayerInputSystem::Run()
{
    Vector3 playerPosition{ 0, 0, 0 };

    //Pacman
    auto view = Engine::Registry().view<Transform, ControllerMapping, MovementData>();
    for (auto entity : view)
    {
        auto &t = view.get<Transform>(entity);
        auto &ctrl = view.get<ControllerMapping>(entity);
        auto &mov = view.get<MovementData>(entity);

        

        t.position.x += ctrl.input.x * mov.maxSpeed;
        t.position.y += ctrl.input.y * mov.maxSpeed;

        playerPosition = t.position;
    }
    //Ghosts
    auto viewEnemy = Engine::Registry().view<EnemyData, Transform, MovementData>();
    for (auto entity : viewEnemy)
    {
        auto& t = viewEnemy.get<Transform>(entity);
        auto& mov = viewEnemy.get<MovementData>(entity);
        auto& enemy = viewEnemy.get<EnemyData>(entity);
        enemy.target = playerPosition;

        Vector3 direction = enemy.target - t.position;
        mov.velocity = direction;
        t.position.x += mov.velocity.x;
        t.position.y += mov.velocity.y;
    }
}
