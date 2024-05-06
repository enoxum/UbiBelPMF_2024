#include "pingpong_playerinput.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include "bullethell_bullet.h"
#include "bullethell_eye.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "gameplay/common/simple_collisions.h"
#include <GLFW/glfw3.h>

using namespace dagger;
using namespace ping_pong;

Float32 PingPongPlayerInputSystem::s_BoarderDown = -20;
Float32 PingPongPlayerInputSystem::s_BoarderUp = 20;

Float32 PingPongPlayerInputSystem::s_PlayerSpeed = 1.5f;

double doubleTapTime = 0.175; //s

void PingPongPlayerInputSystem::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&PingPongPlayerInputSystem::OnKeyboardEvent>(this);
}

void PingPongPlayerInputSystem::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&PingPongPlayerInputSystem::OnKeyboardEvent>(this);
}

void PingPongPlayerInputSystem::OnKeyboardEvent(KeyboardEvent kEvent_)
{

    Engine::Registry().view<ControllerMapping>().each([&](ControllerMapping& ctrl_)
    {
        double currentTime = glfwGetTime();
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



        if (kEvent_.key == ctrl_.right_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
        {
            ctrl_.input.x = 1;
            ctrl_.doubleTap.x = 0;
            if(kEvent_.action == EDaggerInputState::Pressed && currentTime - ctrl_.last_clicked_right <= doubleTapTime){
                ctrl_.doubleTap.x = 1;
                Logger::info("double tap right");
            }

            ctrl_.last_clicked_right = currentTime;

        }
        else if (kEvent_.key == ctrl_.right_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x > 0)
        {
            ctrl_.input.x = 0;
        }
        else if (kEvent_.key == ctrl_.left_key && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
        {
            ctrl_.input.x = -1;
            ctrl_.doubleTap.x = 0;
            if(kEvent_.action == EDaggerInputState::Pressed && currentTime - ctrl_.last_clicked_left <= doubleTapTime){
                Logger::info("double tap left");
                ctrl_.doubleTap.x = -1;
            }
            ctrl_.last_clicked_left = currentTime;

        }
        else if (kEvent_.key == ctrl_.left_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x < 0)
        {
            ctrl_.input.x = 0;
        }


        if (kEvent_.key == ctrl_.eye_shoot && (kEvent_.action == EDaggerInputState::Pressed ) &&
            !ctrl_.eyeShoot)
        {
            ctrl_.eyeShoot = true;
        }
        else if (kEvent_.key == ctrl_.eye_shoot && kEvent_.action == EDaggerInputState::Released && ctrl_.eyeShoot)
        {
            ctrl_.eyeShoot = false;
        }
    });
}

void PingPongPlayerInputSystem::Run()
{
    auto view = Engine::Registry().view<Transform, ControllerMapping, MovementData, Sprite>();
    for (auto entity : view)
    {
        auto &t = view.get<Transform>(entity);
        auto &ctrl = view.get<ControllerMapping>(entity);
        auto &mov = view.get<MovementData>(entity);

        mov.acceleration += ctrl.input.x * s_PlayerSpeed * Engine::DeltaTime();

        if(mov.acceleration > mov.maxSpeed){
            mov.acceleration = mov.maxSpeed;
        }
        if(glm::abs(ctrl.input.x) <= 1e-3 ) {
            mov.acceleration *= 0.99;
            if (glm::abs(mov.acceleration) < 1e-10)
                mov.acceleration = 0.0f;
        }

        mov.angle += ctrl.input.x * s_PlayerSpeed * Engine::DeltaTime();

        ctrl.doubleTap.x *= 0.9;
        if(std::abs(ctrl.doubleTap.x) < 1e-4){
            ctrl.doubleTap.x = 0;
        }
//        ctrl.doubleTap.x = 0;

        auto &spr = view.get<Sprite>(entity);
        spr.rotation = mov.radius* mov.angle;
//        if(mov.angle >= 360.0){
//            mov.angle -= 360.0;
//        }
//        if(mov.angle <= 0){
//            mov.angle += 360.0;
//        }




        t.position.x = mov.radius * glm::cos(mov.angle);
        t.position.y = mov.radius * glm::sin(mov.angle);

        if(ctrl.eyeShoot){
            auto& reg = dagger::Engine::Registry();
            ctrl.eyeShoot = false;
            auto eyeView = Engine::Registry().view<Eye, Transform>();
            auto playerView = Engine::Registry().view<EyeTarget, Transform, MovementData>();

            auto& eyePos = eyeView.get<Transform>(eyeView.front());
            auto& playerPos = playerView.get<Transform>(playerView.front());
            auto& movData = playerView.get<MovementData>(playerView.front());

            auto bullet = reg.create();
            auto& bulletData = reg.emplace<Bullet>(bullet);
            auto& transform = reg.emplace<Transform>(bullet);
            auto& sprite = reg.emplace<Sprite>(bullet);
            reg.emplace<SimpleCollision>(bullet);

            transform.position = playerPos.position;
            auto dir = eyePos.position - playerPos.position;
            bulletData.velocity = glm::normalize(dir);
            bulletData.velocity *= 500;
            bulletData.owner = playerView.front();
            AssignSprite(sprite, "EmptyWhitePixel");
            sprite.size = {1, 1};
            sprite.scale = {5,5};
            sprite.rotation = glm::degrees(movData.angle) + 90.f;
            Logger::info("Bullet");
        }

    }
}
