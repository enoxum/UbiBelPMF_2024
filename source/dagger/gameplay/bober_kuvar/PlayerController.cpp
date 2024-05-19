#include "PlayerController.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include <core/graphics/window.h>
#include <core/graphics/sprite.h>

using namespace bober_game;

double PlayerController::playerSpeed = 100.0;

void PlayerController::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&PlayerController::OnKeyboardEvent>(this);
    Engine::Dispatcher().sink<CursorEvent>().connect<&PlayerController::OnCursorMoveEvent>(this);
    Engine::Dispatcher().sink<MouseEvent>().connect<&PlayerController::OnMouseEvent>(this);
}

void PlayerController::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&PlayerController::OnKeyboardEvent>(this);
    Engine::Dispatcher().sink<CursorEvent>().disconnect<&PlayerController::OnCursorMoveEvent>(this);
    Engine::Dispatcher().sink<MouseEvent>().disconnect<&PlayerController::OnMouseEvent>(this);
}

void PlayerController::OnKeyboardEvent(KeyboardEvent kEvent_)
{
    Engine::Registry().view<ControllerMapping>().each([&](ControllerMapping& ctrl_)
    {
        if (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held)
        {
            if (kEvent_.key == ctrl_.upKey)
                ctrl_.input.y = 1;
            if (kEvent_.key == ctrl_.downKey)
                ctrl_.input.y = -1;

            if (kEvent_.key == ctrl_.leftKey)
                ctrl_.input.x = -1;
            if (kEvent_.key == ctrl_.rightKey)
                ctrl_.input.x = 1;
        }
        else if (kEvent_.action == EDaggerInputState::Released)
        {
            if(kEvent_.key == ctrl_.upKey || kEvent_.key == ctrl_.downKey)
                ctrl_.input.y = 0;

            if (kEvent_.key == ctrl_.leftKey || kEvent_.key == ctrl_.rightKey)
                ctrl_.input.x = 0;
        }
    });
}

void PlayerController::OnCursorMoveEvent(CursorEvent cursor_)
{
    auto viewCursor = Engine::Registry().view<Cursor>();
    for (auto entity : viewCursor)
    {
        auto& c = viewCursor.get<Cursor>(entity);
        auto* camera = Engine::GetDefaultResource<Camera>();

        c.position.x = cursor_.x - camera->size.x / 2;
        c.position.y = -cursor_.y + camera->size.y / 2;
    }
}

void PlayerController::OnMouseEvent(MouseEvent input_)
{
    bool isMousePressed = input_.action == EDaggerInputState::Pressed;

    auto viewCursor = Engine::Registry().view<Cursor>();
    for (auto entity : viewCursor)
    {
        auto& c = viewCursor.get<Cursor>(entity);
        c.isMouseBtnPressed = isMousePressed;
    }
}

void PlayerController::Run()
{
    auto view = Engine::Registry().view<Transform, ControllerMapping>();
    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& ctrl = view.get<ControllerMapping>(entity);

        if (ctrl.input == Vector2{0.0f, 0.0f})
            break;
        double normalized = 1 / sqrt(ctrl.input.x * ctrl.input.x + ctrl.input.y * ctrl.input.y);

        t.position.x += normalized * ctrl.input.x * playerSpeed * Engine::DeltaTime();
        t.position.y += normalized * ctrl.input.y * playerSpeed * Engine::DeltaTime();
    }
    auto viewCursor = Engine::Registry().view<Transform, Sprite, Cursor>();
    for (auto entity : viewCursor)
    {
        auto& t = viewCursor.get<Transform>(entity);
        auto& s = viewCursor.get<Sprite>(entity);
        auto& c = viewCursor.get<Cursor>(entity);

        t.position.x = c.position.x;
        t.position.y = c.position.y;

        s.color.g = c.isMouseBtnPressed ? 1 : 0;
    }
}
