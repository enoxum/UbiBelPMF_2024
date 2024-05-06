//
// Created by igor on 4/18/24.
//
#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include <glm/common.hpp>
#include <GLFW/glfw3.h>
#include "bullethell_eye.h"
#include "gameplay/ping_pong/pingpong_playerinput.h"


using namespace ping_pong;

void EyeSystem::Run() {
    auto view = Engine::Registry().view<MovementData, EyeTarget>();
    auto enemy = Engine::Registry().view<Eye, Sprite>();
    for(auto entity : view)
    {
        for(auto eye : enemy){
            auto& mov = view.get<MovementData>(entity);

            auto& spr = enemy.get<Sprite>(eye);
            auto eyeAngle = spr.rotation;

//            Logger::info("{} {}", eyeAngle, mov.angle);

            auto diff = spr.rotation - glm::degrees(mov.angle) - 90.f;
            auto speed = -diff * Engine::DeltaTime();
            if(std::abs(diff) < 1e-3)
                speed = 0;
            spr.rotation += speed;

        }
    }
}


