#include "PlayerController.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include <gameplay/common/simple_collisions.h>

using namespace bober_game;

double PlayerController::playerSpeed = 1000.0;
Vector2 playerPos = { 0.0, 0.0 };

void PlayerController::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&PlayerController::OnKeyboardEvent>(this);
}

void PlayerController::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&PlayerController::OnKeyboardEvent>(this);
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

void PlayerController::Run()
{
    auto otherViews = Engine::Registry().view<Transform, SimpleCollision>();
    auto view = Engine::Registry().view<Transform, ControllerMapping, SimpleCollision>();
    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& ctrl = view.get<ControllerMapping>(entity);
        auto& col = view.get<SimpleCollision>(entity);

        if (!col.colided) {
            playerPos = t.position;
        }

        if (ctrl.input == Vector2{0.0f, 0.0f})
            break;
        double normalized = 1 / sqrt(ctrl.input.x * ctrl.input.x + ctrl.input.y * ctrl.input.y);

        double move_x = normalized * ctrl.input.x * playerSpeed * Engine::DeltaTime();
        double move_y = normalized * ctrl.input.y * playerSpeed * Engine::DeltaTime();

        t.position.x += move_x;
        t.position.y += move_y;

        if (col.colided) {
            if (Engine::Registry().valid(col.colidedWith))
            {
                SimpleCollision& collision = otherViews.get<SimpleCollision>(col.colidedWith);
                Transform& transform = otherViews.get<Transform>(col.colidedWith);
                Vector2 collisionSides = col.GetCollisionSides(t.position, collision, transform.position);


                do {
                    if (std::abs(collisionSides.x) > 0)
                        t.position.x -= collisionSides.x * Engine::DeltaTime();

                    if (std::abs(collisionSides.y) > 0)
                        t.position.y -= collisionSides.y * Engine::DeltaTime();
                } while (col.IsCollided(t.position, collision, transform.position));

                //t.position = { playerPos, 0.0 };
            }
        }

        col.colided = false;
    }
}
