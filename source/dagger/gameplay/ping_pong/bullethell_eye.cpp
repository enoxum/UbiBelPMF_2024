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

float elapsed_time = 0.f;
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

//            Logger::info("{} {}", eyeAngle, mov.angle);

            auto diff = spr.rotation - glm::degrees(mov.angle) - 90.f;
            auto speed = -diff * Engine::DeltaTime();
            if (std::abs(diff) < 1e-3)
                speed = 0;
            spr.rotation += speed;

            auto &t = view.get<Transform>(entity);
            auto &ctrl = view.get<ControllerMapping>(entity);
//            auto &mov = view.get<MovementData>(entity);

            auto &reg = dagger::Engine::Registry();
            ctrl.eyeShoot = false;
            auto eyeView = Engine::Registry().view<Eye, Transform>();
            auto playerView = Engine::Registry().view<EyeTarget, Transform, MovementData>();

            auto &eyePos = eyeView.get<Transform>(eyeView.front());
            auto &playerPos = playerView.get<Transform>(playerView.front());
            auto &movData = playerView.get<MovementData>(playerView.front());

            elapsed_time += Engine::DeltaTime();

//                auto bullet4 = reg.create();
//                auto &bulletData4 = reg.emplace<Bullet>(bullet4);
//                auto &transform4 = reg.emplace<Transform>(bullet4);
//                auto &sprite4 = reg.emplace<Sprite>(bullet4);
//                reg.emplace<SimpleCollision>(bullet4);
//
//                transform4.position.x = -45.f + bulletData4.boss_radius * glm::cos(mov.angle);
//                transform4.position.y = bulletData4.boss_radius * glm::sin(mov.angle);
//
//                auto dir4 = playerPos.position - eyePos.position;
//                auto auxiliary = dir4.x;
//                dir4.x = -dir4.x;
//                dir4.y = -dir4.y;
//                bulletData4.velocity = glm::normalize(dir4);
//                bulletData4.velocity *= 200;
//                bulletData4.owner = playerView.front();
//                AssignSprite(sprite4, "EmptyWhitePixel");
//                sprite4.size = {3,3};
//                sprite4.scale = {5, 5};

            auto bullet = reg.create();
            auto &bulletData = reg.emplace<Bullet>(bullet);
            auto &bullet_transform = reg.emplace<Transform>(bullet);
            auto &bullet_sprite = reg.emplace<Sprite>(bullet);
            AssignSprite(bullet_sprite, "EmptyWhitePixel");
            bullet_sprite.size = {3,3};
            bullet_sprite.scale = {5,5};
            auto speed_ = 100;
//            bullet_transform.position.x = bullet_transform.position.x*speed*Engine::DeltaTime();

            float shoot_timer=0.2f;
            float rotate_speed = 100;
            int spawn_point_count = 4;
            int radius = 100;

            if(counter == 4)
                counter=0;

//            float step = 2 * M_PI / spawn_point_count;
//            auto spawn_point = rotateVector(Vector2(radius, 0), step * counter);
//            auto spawn_point_3D = Vector3(spawn_point, 0);
//            auto direction = spawn_point_3D;
//                bulletData.velocity = glm::normalize(direction);
//                bulletData.velocity *= 5;
//                bullet_transform.position = spawn_point_3D;
//            float angle = angleBetweenVectors(spawn_point, fixed_vector);
//                AssignSprite(bullet_sprite, "EmptyWhitePixel");
//                bullet_sprite.size = {3, 3};
//                bullet_sprite.scale = {5, 5};
//                std::cout << spawn_point[0] << " || " << spawn_point[1] << " ANGLE: " << angle << "\n";
//
//                bullet_transform.position.x += 50 * sin(glfwGetTime());
//                bullet_transform.position.y += 50 * cos(glfwGetTime());
//                elapsed_time = 0;

//            bullet_sprite.rotation =

            for(int i=0; i<4; i++) {
                if(elapsed_time < 0.3)
                    break;
                auto bullet3 = reg.create();
                auto &bulletData3 = reg.emplace<Bullet>(bullet3);
                auto &transform3 = reg.emplace<Transform>(bullet3);
                auto &sprite3 = reg.emplace<Sprite>(bullet3);
                reg.emplace<SimpleCollision>(bullet3);

                float step = 2 * M_PI / spawn_point_count;
                auto spawn_point = rotateVector(Vector2(radius, 0), step * i);
                auto spawn_point_3D = Vector3(spawn_point, 0);
                auto direction = spawn_point_3D;
                float angle = angleBetweenVectors(spawn_point, fixed_vector);

                transform3.position = spawn_point_3D;
//                transform3.position.y = 10 * bulletData3.boss_radius * glm::sin(glfwGetTime()) + 3 * glfwGetTime();

                auto dir3 = direction;
                bulletData3.velocity = glm::normalize(dir3);
                bulletData3.velocity *= 200;
                bulletData3.owner = playerView.front();
                AssignSprite(sprite3, "EmptyWhitePixel");
                sprite3.size = {3, 3};
                sprite3.scale = {5, 5};
                if (i==3)
                    elapsed_time=0;
            }

                // TILL HERE


//                auto bullet4 = reg.create();
//                auto &bulletData4 = reg.emplace<Bullet>(bullet4);
//                bulletData4.owner = eyeView.front();
//                auto &transform4 = reg.emplace<Transform>(bullet4);
//                auto &sprite4 = reg.emplace<Sprite>(bullet4);
//                reg.emplace<SimpleCollision>(bullet4);
//
//                transform4.position.x = -45.f + bulletData4.boss_radius * glm::cos(mov.angle);
//                transform4.position.y = bulletData4.boss_radius * glm::sin(mov.angle);
//
//                auto dir4 = playerPos.position - eyePos.position;
//                auto auxiliary = dir4.x;
//                dir4.x = -dir4.x;
//                dir4.y = -dir4.y;
//                bulletData4.velocity = glm::normalize(dir4);
//                bulletData4.velocity *= 200;
//                bulletData4.owner = playerView.front();
//                AssignSprite(sprite4, "EmptyWhitePixel");
//                sprite4.size = {3,3};
//                sprite4.scale = {5, 5};
//
//                auto bullet3 = reg.create();
//                auto &bulletData3 = reg.emplace<Bullet>(bullet3);
//                bulletData3.owner = eyeView.front();
//                auto &transform3 = reg.emplace<Transform>(bullet3);
//                auto &sprite3 = reg.emplace<Sprite>(bullet3);
//                reg.emplace<SimpleCollision>(bullet3);
//
//                transform3.position.x = bulletData3.boss_radius * glm::cos(mov.angle);
//                transform3.position.y = bulletData3.boss_radius * glm::sin(mov.angle);
//
//                auto dir3 = eyePos.position -  playerPos.position;
//                auxiliary = dir3.x;
//                dir3.x = -dir3.y;
//                dir3.y = auxiliary;
//                bulletData3.velocity = glm::normalize(dir3);
//                bulletData3.velocity *= 200;
//                bulletData3.owner = playerView.front();
//                AssignSprite(sprite3, "EmptyWhitePixel");
//                sprite3.size = {3,3};
//                sprite3.scale = {5, 5};
//
//
//                auto bullet2 = reg.create();
//                auto &bulletData2 = reg.emplace<Bullet>(bullet2);
//                bulletData2.owner = eyeView.front();
//                auto &transform2 = reg.emplace<Transform>(bullet2);
//                auto &sprite2 = reg.emplace<Sprite>(bullet2);
//                reg.emplace<SimpleCollision>(bullet2);
//
//                transform2.position.x = bulletData2.boss_radius * glm::cos(mov.angle);
//                transform2.position.y = bulletData2.boss_radius * glm::sin(mov.angle);
//
//                auto dir2 = eyePos.position -  playerPos.position;
//                dir2.x *= -1;
//                auxiliary = dir2.x;
//                dir2.x = dir2.y;
//                dir2.y = auxiliary;
//                bulletData2.velocity = glm::normalize(dir2);
//                bulletData2.velocity *= 200;
//                bulletData2.owner = playerView.front();
//                AssignSprite(sprite2, "EmptyWhitePixel");
//                sprite2.size = {3,3};
//                sprite2.scale = {5, 5};
//
//                auto bullet = reg.create();
//                auto &bulletData = reg.emplace<Bullet>(bullet);
//                bulletData.owner = eyeView.front();
//                auto &transform = reg.emplace<Transform>(bullet);
//                auto &sprite = reg.emplace<Sprite>(bullet);
//                reg.emplace<SimpleCollision>(bullet);
//
////                double angle = atan2(eyePos.position.y - playerPos.position.y,
////                                     eyePos.position.x - playerPos.position.x);
//                transform.position.x = 100.f * cos(glfwGetTime());
//                transform.position.y = 100.f * cos(glfwGetTime());
//
//                auto dir = playerPos.position - eyePos.position;
////                std::cout << "DIR.x: " << dir.x << '\n' << "DIR.y: " << dir.y << '\n';
////                std::cout << "ANGLE" << glm::degrees(mov.angle) << std::endl;
//                bulletData.velocity = glm::normalize(dir);
//                bulletData.velocity *= 200;
//                bulletData.owner = playerView.front();
//                AssignSprite(sprite, "EmptyWhitePixel");
//                sprite.size = {3,3};
//                sprite.scale = {5, 5};
//
//                auto &col = view.get<SimpleCollision>(entity);
//
//                if (col.colided) {
//                    if (Engine::Registry().valid(col.colidedWith)) {
//                        SimpleCollision &collision = view.get<SimpleCollision>(col.colidedWith);
//                        // what do we want here?
//                        std::cout << "COLIDED" << std::endl;
//                    }
//                }
//
//                sprite.rotation = glm::degrees(movData.angle) + 90.f;
//                Logger::info("Bullet");
//                elapsed_time = 0;
            }
        }

    }
//}


//
//            if(elapsed_time > 0.3f) {
//                auto bullet = reg.create();
//                auto &bulletData = reg.emplace<Bullet>(bullet);
//                auto &transform = reg.emplace<Transform>(bullet);
//                auto &sprite = reg.emplace<Sprite>(bullet);
//                reg.emplace<SimpleCollision>(bullet);
//
//                // code for pattern
//
//                // till here
//
//                transform.position.x = 50.f * glm::cos(glm::degrees(mov.angle));
//                transform.position.y = 50.f * glm::sin(glm::degrees(mov.angle));
//                auto dir = playerPos.position - eyePos.position;
//                std::cout << "DIR.x: " << playerPos.position.x << '\n' << "DIR.y: " << playerPos.position.y << '\n';
//                std::cout << "DIR.x: " << eyePos.position.x << '\n' << "DIR.y: " << eyePos.position.y << '\n';
//                std::cout << "ANGLE" << glm::degrees(mov.angle) << std::endl;
//                bulletData.velocity = glm::normalize(dir);
//                bulletData.velocity *= 500;
//                bulletData.owner = playerView.front();
//                AssignSprite(sprite, "EmptyWhitePixel");
//                sprite.size = {3, 3};
//                sprite.scale = {5, 5};


//
//                float spawn_point_count = 4.f;
//                float radius_pattern = 100.f;

////                 equal distance between bullets on circle
//                float step = 2 * M_PI/spawn_point_count;
//                for(int i=0; i<4; i++){
//                    auto bullets = reg.create();
//                    auto &bulletsData = reg.emplace<Bullet>(bullets);
//                    auto &transform_bullets = reg.emplace<Transform>(bullets);
//                    auto &sprite_bullets = reg.emplace<Sprite>(bullets);
//                    reg.emplace<SimpleCollision>(bullets);
//
//                    // code for pattern
//
//                    // till here
//
////                    transform_bullets.position = (transform.position.x + glm::sin(180.f*i),transform.position.y+ glm::cos(180.f*i));
////                    transform_bullets.position.x = transform.position.x;
////                    transform_bullets.position.y = transform.position.y;
//                    transform_bullets.position.x = bulletData.boss_radius * glm::cos(mov.angle + 90.f*i);
//                    transform_bullets.position.y = bulletData.boss_radius * glm::sin(mov.angle + 90.f*i);
//
////                    auto dir_bullets = playerPos.position - eyePos.position + (glm::cos(step*i), glm::sin(step*i));
//                    auto dir_bullets = dir + (90.f, 90.f);
//                    bulletsData.velocity = glm::normalize(dir_bullets);
//                    bulletsData.velocity *= 500;
//                    bulletsData.owner = playerView.front();
//                    AssignSprite(sprite_bullets, "EmptyWhitePixel");
//                    sprite_bullets.size = {3,3};
//                    sprite_bullets.scale = {5, 5};
//                }
