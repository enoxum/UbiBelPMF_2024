#include "deadend_playerinput.h"
#include "deadend_player.h"
#include "deadend_crosshair.h"
#include "deadend_bullet.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include "core/input/inputs.h"
#include "core/graphics/window.h"

#include "gameplay/common/simple_collisions.h"
#include "deadend_camera.h"
#include <iostream>

using namespace dagger;
using namespace dead_end;

void dead_end::DeadEndPlayerInputSystem::SpinUp()
{
	Engine::Dispatcher().sink<KeyboardEvent>().connect<&DeadEndPlayerInputSystem::OnKeyboardEvent>(this);
}

void dead_end::DeadEndPlayerInputSystem::WindDown()
{
	Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&DeadEndPlayerInputSystem::OnKeyboardEvent>(this);
}

void dead_end::DeadEndPlayerInputSystem::OnKeyboardEvent(KeyboardEvent kEvent_)
{
    Engine::Registry().view<ControllerMapping>().each([&](ControllerMapping& ctrl_)
        {
            // moving
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
            else if (kEvent_.key == ctrl_.left_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
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

            // shooting
            if (kEvent_.key == ctrl_.shoot_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
            {
                ctrl_.shooting = true;
            }
            else if (kEvent_.key == ctrl_.shoot_key && kEvent_.action == EDaggerInputState::Released && ctrl_.shooting == true)
            {
                ctrl_.shooting = false;
            }
        });
}



void dead_end::DeadEndPlayerInputSystem::Run()
{

    auto view = Engine::Registry().view<Transform, ControllerMapping, Player, SimpleCollision, Sprite, DeadEndCamera>();
    auto viewAim = Engine::Registry().view<Aim>();

    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& ctrl = view.get<ControllerMapping>(entity);
        auto& player = view.get<Player>(entity);
        auto& col = view.get<SimpleCollision>(entity);
        auto& s = view.get<Sprite>(entity);
        auto& camera = view.get<DeadEndCamera>(entity);


        t.position.x += ctrl.input.x * Engine::DeltaTime() * player.speed;
        t.position.y += ctrl.input.y * Engine::DeltaTime() * player.speed;

        if (player.weaponType == 1)
        {
            // assign a sprite for a player w/pistol.
        }
        else {
            // assign a sprite for a player w/gun.
        }


        if (ctrl.shooting && player.weaponType != 0)
        {
            auto* config = Engine::GetDefaultResource<RenderConfig>();
            auto* c = Engine::GetDefaultResource<Camera>();
            
            auto cursor = dagger::Input::CursorPositionInWindow();

            //Vector2 screenCenter{400, 300}; // width / 2, height / 2
            Vector2 screenCenter{camera.size.x / 2, camera.size.y / 2}; // width / 2, height / 2

            Vector2 relativePosition = cursor - screenCenter; 


            Vector2 windowCoord{ 0,0 }; 
            windowCoord.x += camera.position.x + relativePosition.x;
            windowCoord.y += camera.position.y - relativePosition.y;

            Vector2 position = { t.position.x, t.position.y };
            Vector2 target = windowCoord;


            dead_end::CreateBullet(position, target, player.weaponType);
            
            ctrl.shooting = false;
        }
    }
}

