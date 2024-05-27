#include "deadend_enemy.h"
#include "deadend_player.h"
#include "deadend_health.h"
#include "deadend_wavestruct.h"
#include "deadend_bullet.h"
#include "deadend_obstacle.h"

#include "collectables.h"

#include "core/engine.h"
#include "core/core.h"
#include "core/game/transforms.h"
#include "core/graphics/text.h"
#include "core/graphics/sprite.h"

#include "gameplay/common/simple_collisions.h"


#include <algorithm>
#include <vector>
#include <math.h>

using namespace dagger;
using namespace dead_end;

void dead_end::DeadEndEnemyMoveSystem::Run()
{

    auto enemyView = Engine::Registry().view<DeadEndEnemy, Transform, SimpleCollision, Health, Sprite>();
    auto playerView = Engine::Registry().view<Transform, Player>();
    auto viewCollisions = Engine::Registry().view<DeadEndObstacle, Transform, SimpleCollision>();


    for (auto entity : enemyView)
    {
        auto &enemyPos = enemyView.get<Transform>(entity);
        auto &enemy = enemyView.get<DeadEndEnemy>(entity);
        auto& enemySprite = enemyView.get<Sprite>(entity);
        auto& enemyCol = enemyView.get<SimpleCollision>(entity);

        auto playerEntity = playerView.front();
        auto& playerPos = playerView.get<Transform>(playerEntity);

       
        Vector2 direction = { playerPos.position.x - enemyPos.position.x, playerPos.position.y - enemyPos.position.y };

        float length = sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0.0f)
        {
            direction.x /= length;
            direction.y /= length;
        }

        if (enemyCol.colided)
        {

            if (Engine::Registry().valid(enemyCol.colidedWith) && enemyView.contains(enemyCol.colidedWith))
            {
                enemyPos.position.x += direction.x * Engine::DeltaTime() * enemy.speed;
                enemyPos.position.y += direction.y * Engine::DeltaTime() * enemy.speed;
                continue;
            }

            if (Engine::Registry().valid(enemyCol.colidedWith) && viewCollisions.contains(enemyCol.colidedWith))
            {
                SimpleCollision& collision = viewCollisions.get<SimpleCollision>(enemyCol.colidedWith);
                Transform& transform = viewCollisions.get<Transform>(enemyCol.colidedWith);

                Vector2 collisionSides = enemyCol.GetCollisionSides(enemyPos.position, collision, transform.position);

                do
                {
                    // get back for 1 frame 
                    Float32 dt = Engine::DeltaTime();

                    if (collisionSides.x > 0)
                    {
                        enemyPos.position.x -= direction.x * enemy.speed * dt;
                    }
                    else if (collisionSides.x < 0)
                    {
                        enemyPos.position.x += direction.x * enemy.speed * dt;
                    }

                    if (collisionSides.y > 0)
                    {
                        enemyPos.position.y -= direction.y * enemy.speed * dt;
                    }
                    else if (collisionSides.y < 0)
                    {
                        enemyPos.position.y += direction.y * enemy.speed * dt;
                    }

                } while (enemyCol.IsCollided(enemyPos.position, collision, transform.position));
                
            }
            enemyCol.colided = false;
        }

        enemyPos.position.x += direction.x * Engine::DeltaTime() * enemy.speed;
        enemyPos.position.y += direction.y * Engine::DeltaTime() * enemy.speed;
       

      
        float angleRadians = atan2(direction.y, direction.x);
        float angleDegs = glm::degrees(angleRadians) - 90.0f; 

        enemySprite.rotation = angleDegs;

    }
   
}

void dead_end::DeadEndEnemyBulletCollisionSystem::Run()
{
    auto enemyView = Engine::Registry().view<DeadEndEnemy, Transform, SimpleCollision, Health, Sprite>();
    auto bulletView = Engine::Registry().view<Bullet, SimpleCollision>();

    std::vector<Entity> bulletsToDestroy;
    std::vector<Entity> enemiesToDestroy;

    for (auto bulletEntity : bulletView)
    {
        auto& bullet = bulletView.get<Bullet>(bulletEntity);
        auto& bulletCol = bulletView.get<SimpleCollision>(bulletEntity);

        if (bulletCol.colided)
        {

            if (Engine::Registry().valid(bulletCol.colidedWith) && enemyView.contains(bulletCol.colidedWith))
            {
                auto& enemy = enemyView.get<DeadEndEnemy>(bulletCol.colidedWith);
                auto& enemyHp = enemyView.get<Health>(bulletCol.colidedWith);

                enemyHp.currentHealth -= bullet.damage;
                if (enemyHp.currentHealth <= 0) 
                {
                    enemy.is_dead = true;
                    enemiesToDestroy.push_back(bulletCol.colidedWith);
                }
                bulletCol.colided = false;
                bulletsToDestroy.push_back(bulletEntity);
                break;
            }
        }
        
    }

    for (auto bulletEntity : bulletsToDestroy) {
        if (Engine::Registry().valid(bulletEntity)) {
            Engine::Registry().destroy(bulletEntity);
        }
    }

    for (auto enemyEntity : enemiesToDestroy) {
        if (Engine::Registry().valid(enemyEntity)) {
            Engine::Registry().destroy(enemyEntity);
        }
    }
}

