//
// Created by igor on 4/18/24.
//
#include "gameplay/common/simple_collisions.h"
#include "bullethell_bullet.h"
#include "bullethell_eye.h"
#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include <glm/common.hpp>
#include <GLFW/glfw3.h>
#include <thread>
#include <iostream>
#include "bullethell_eye.h"
#include "gameplay/ping_pong/pingpong_playerinput.h"
#include <cmath>


using namespace ping_pong;

Vector2 rotateVector(const Vector2 vector, float angle) {
    float rotationMatrix[2][2] = {
            {float(cos(angle)), float(-sin(angle))},
            {float(sin(angle)), float(cos(angle))}
    };

    // new coordinates
    float x = vector.x * rotationMatrix[0][0] + vector.y * rotationMatrix[0][1];
    float y = vector.x * rotationMatrix[1][0] + vector.y * rotationMatrix[1][1];

    return Vector2(x, y);
}

float angleBetweenVectors(const Vector2& a, const Vector2& b) {
    float dotProduct = a.x * b.x + a.y * b.y;

    // vector length
    float magnitudeA = sqrt(a.x * a.x + a.y * a.y);
    float magnitudeB = sqrt(b.x * b.x + b.y * b.y);

    float cosAngle = dotProduct / (magnitudeA * magnitudeB);

    float angleRad = acos(cosAngle);

    float angleDeg = angleRad * 180 / M_PI;

    if(a.y < -9)
        angleDeg = 360 - angleDeg;
    return angleDeg;
}

Vector2 fixed_vector(100.f, 0.f);
int radius = 100;
Vector2 rotated_vector = Vector2(radius,0);

float elapsed_time = 0.f;
float elapsed_time2 = 0.f;
int counter = 0;
void EyeSystem::Run() {
    auto view = Engine::Registry().view<SimpleCollision,Transform, ControllerMapping, MovementData, Sprite, EyeTarget>();
    auto enemy = Engine::Registry().view<Eye, Sprite>();
    for (auto entity: view) {
        for (auto eye: enemy) {
            auto &mov = view.get<MovementData>(entity);
            counter++;

            auto &spr = enemy.get<Sprite>(eye);
            auto eyeAngle = spr.rotation;
            auto enemy_eye = enemy.get<Eye>(eye);

            auto diff = spr.rotation - glm::degrees(mov.angle) - 90.f;
            auto speed = -diff * Engine::DeltaTime();
            if (std::abs(diff) < 1e-3)
                speed = 0;
            spr.rotation += speed;

            auto &t = view.get<Transform>(entity);
            auto &ctrl = view.get<ControllerMapping>(entity);

            auto &reg = dagger::Engine::Registry();
            ctrl.eyeShoot = false;
            auto eyeView = Engine::Registry().view<Eye, Transform>();
            auto playerView = Engine::Registry().view<EyeTarget, Transform, MovementData>();

            auto &eyePos = eyeView.get<Transform>(eyeView.front());
            auto &playerPos = playerView.get<Transform>(playerView.front());
            auto &movData = playerView.get<MovementData>(playerView.front());
            auto EyeStats = Engine::Registry().try_get<StatsData>(eye);


            elapsed_time += Engine::DeltaTime();
            elapsed_time2 += Engine::DeltaTime();


            auto bullet = reg.create();
            auto &bulletData = reg.emplace<Bullet>(bullet);
            auto &bullet_transform = reg.emplace<Transform>(bullet);
            auto &bullet_sprite = reg.emplace<Sprite>(bullet);
            AssignSprite(bullet_sprite, "EmptyWhitePixel");
            bullet_sprite.size = {3, 3};
            bullet_sprite.scale = {5, 5};

            int spawn_point_count = 24;

            if (EyeStats->hp > 80.f) {
                if (elapsed_time >= 0.3) {
                    rotated_vector = rotateVector(rotated_vector, M_PI_2 / 6.f); // angle = 15 deg
                    for (int i = 0; i < 4; i++) {
                        auto bullet3 = reg.create();
                        auto &bulletData3 = reg.emplace<Bullet>(bullet3);
                        auto &transform3 = reg.emplace<Transform>(bullet3);
                        auto &sprite3 = reg.emplace<Sprite>(bullet3);
                        reg.emplace<SimpleCollision>(bullet3);

                        float step = 2 * M_PI / spawn_point_count;
                        auto spawn_point = rotateVector(rotated_vector, i * M_PI_2);
                        auto direction = Vector3(spawn_point, 0);
                        float angle = angleBetweenVectors(spawn_point, fixed_vector);

                        transform3.position = direction;

                        auto dir3 = direction;
                        bulletData3.velocity = glm::normalize(dir3);
                        bulletData3.velocity *= 200;
                        bulletData3.owner = playerView.front();
                        AssignSprite(sprite3, "EmptyWhitePixel");
                        sprite3.size = {3, 3};
                        sprite3.scale = {5, 5};
                    }
                    elapsed_time = 0;
                }
            }
        }
    }

}