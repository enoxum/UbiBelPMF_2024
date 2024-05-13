#include "player.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

#include "gameplay/common/simple_collisions.h"

#include <algorithm>
#include <execution>

using namespace dagger;
using namespace esccape;

void Player::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&Player::OnKeyboardEvent>(this);
}

void Player::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&Player::OnKeyboardEvent>(this);
}

void Player::OnKeyboardEvent(KeyboardEvent kEvent_)
{
    Engine::Registry().view<ControllerMapping>().each([&](ControllerMapping& ctrl_)
        {

            if (kEvent_.key == ctrl_.leftKey && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
            {
                ctrl_.input.x = -1;
            }
            else if (kEvent_.key == ctrl_.leftKey && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x < 0)
            {
                ctrl_.input.x = 0;
            }
            else if (kEvent_.key == ctrl_.rightKey && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
            {
                ctrl_.input.x = 1;
            }
            else if (kEvent_.key == ctrl_.rightKey && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x > 0)
            {
                ctrl_.input.x = 0;
            }
            else if (kEvent_.key == ctrl_.downKey && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
            {
                ctrl_.input.y = -1;
            }
            else if (kEvent_.key == ctrl_.downKey && kEvent_.action == EDaggerInputState::Released && ctrl_.input.y < 0)
            {
                ctrl_.input.y = 0;
            }
            else if (kEvent_.key == ctrl_.upKey && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
            {
                ctrl_.input.y = 1;
            }
            else if (kEvent_.key == ctrl_.upKey && kEvent_.action == EDaggerInputState::Released && ctrl_.input.y > 0)
            {
                ctrl_.input.y = 0;
            }
        });
}

void Player::Run()
{
    //RacingGameFieldSettings fieldSettings;
    //if (auto* ptr = Engine::GetDefaultResource<RacingGameFieldSettings>())
    //{
    //    fieldSettings = *ptr;
    //}

    auto viewCollisions = Engine::Registry().view<Transform, SimpleCollision>();
    auto view = Engine::Registry().view<Transform, ControllerMapping, PlayerEntity>();
    auto view2 = Engine::Registry().view<PlayerEntity, Transform, SimpleCollision>();

    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& ctrl = view.get<ControllerMapping>(entity);
        auto& player = view.get<PlayerEntity>(entity);

        auto& col = view2.get<SimpleCollision>(entity);

        t.position.x += ctrl.input.x * player.speed * Engine::DeltaTime();
        t.position.y += ctrl.input.y * player.speed * Engine::DeltaTime();

        Logger::trace(t.position.x);

        if (col.colided)
        {
            if (Engine::Registry().valid(col.colidedWith))
            {
                SimpleCollision& collision = viewCollisions.get<SimpleCollision>(col.colidedWith);
                Transform& transform = viewCollisions.get<Transform>(col.colidedWith);

                Vector2 collisionSides = col.GetCollisionSides(t.position, collision, transform.position);

                Logger::trace(collisionSides.y);

               // if (col.IsCollided(t.position, collision, transform.position))
                while(col.IsCollided(t.position, collision, transform.position))
                {
                    // get back for 1 frame 
                   
                    if (std::abs(collisionSides.x) > 0)
                    {
                        t.position.x -= t.position.x < 0 ? -1 : 1;
                    }
                    if (std::abs(collisionSides.y) > 0 && col.IsCollided(t.position, collision, transform.position))
                    {
                        t.position.y -= t.position.y < 0 ? -1 : 1;
                    }
                }
            }
        }


        /*Float32 boarderX = fieldSettings.GetXBoarder();
        if (t.position.x > boarderX)
        {
            t.position.x = boarderX;
        }

        if (t.position.x < -boarderX)
        {
            t.position.x = -boarderX;
        }*/
    }
}

