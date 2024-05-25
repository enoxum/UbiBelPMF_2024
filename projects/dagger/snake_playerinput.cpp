#include "snake_playerinput.h"
#include "core/engine.h"
#include "core/game/transforms.h"
#include <iostream>

using namespace dagger;
using namespace red_snake;

Float32 SnakePlayerInputSystem::s_SnakeSpeed = 5.0f;

void SnakePlayerInputSystem::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&SnakePlayerInputSystem::OnKeyboardEvent>(this);
    std::cout << "SnakePlayerInputSystem spun up" << std::endl;
}

void SnakePlayerInputSystem::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&SnakePlayerInputSystem::OnKeyboardEvent>(this);
    std::cout << "SnakePlayerInputSystem winded down" << std::endl;
}

void SnakePlayerInputSystem::OnKeyboardEvent(KeyboardEvent kEvent_)
{
    std::cout << "Keyboard event detected: " << static_cast<int>(kEvent_.key) << std::endl;

    Engine::Registry().view<SnakeControllerMapping>().each([&](SnakeControllerMapping& ctrl_)
        {
            if (kEvent_.key == ctrl_.up_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
            {
                ctrl_.input.y = 1;
            }
            else if (kEvent_.key == ctrl_.up_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.y > 0)
            {
                ctrl_.input.y = 0;
            }
            else if (kEvent_.key == ctrl_.down_key && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
            {
                ctrl_.input.y = -1;
            }
            else if (kEvent_.key == ctrl_.down_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.y < 0)
            {
                ctrl_.input.y = 0;
            }
            else if (kEvent_.key == ctrl_.left_key && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
            {
                ctrl_.input.x = -1;
            }
            else if (kEvent_.key == ctrl_.left_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x < 0)
            {
                ctrl_.input.x = 0;
            }
            else if (kEvent_.key == ctrl_.right_key && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
            {
                ctrl_.input.x = 1;
            }
            else if (kEvent_.key == ctrl_.right_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x > 0)
            {
                ctrl_.input.x = 0;
            }
        });
}

void SnakePlayerInputSystem::Run()
{
    auto view = Engine::Registry().view<Transform, SnakeControllerMapping>();
    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& ctrl = view.get<SnakeControllerMapping>(entity);

        Vector3 direction{ ctrl.input.x, ctrl.input.y, 0 };
        

        if (t.position.x >= (26 / 2 + 0.5f) * 20.f  + 5 ||
            t.position.x <= -(26 / 2 + 0.5f) * 20.f -5 ||
            t.position.y >= (20 / 2 + 0.5f) * 20.f + 5 ||
            t.position.y <= -(20 / 2 + 0.5f) * 20.f - 5
            ) {
            HWND hwnd = CreateWindow("myWindowClass", "Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                100, 100, 500, 400, NULL, NULL, NULL, NULL);
            int result = MessageBox(hwnd, "Game Over!", "RedSnake", MB_OK);

            if (result == IDOK)
            {
                PostQuitMessage(0);
            }
        }

        
        t.position += direction * SnakePlayerInputSystem::s_SnakeSpeed * 30.0f * Engine::DeltaTime(); 
    }
}
