#include "deadend_enemy.h"
#include "deadend_player.h"
#include "deadend_health.h"
#include "deadend_wavestruct.h"

#include "collectables.h"

#include "core/engine.h"
#include "core/core.h"
#include "core/game/transforms.h"
#include "core/graphics/text.h"

#include "gameplay/common/simple_collisions.h"


#include <algorithm>
#include <math.h>
#include "deadend_bullet.h"

using namespace dagger;
using namespace dead_end;

void dead_end::DeadEndEnemySystem::Run()
{
    auto playerView = Engine::Registry().view<Player, Transform, SimpleCollision, Health>();
    auto enemyView = Engine::Registry().view<DeadEndEnemy, Transform, SimpleCollision, Health>();
    auto bulletView = Engine::Registry().view<Bullet, SimpleCollision>();
    auto viewCollisions = Engine::Registry().view<Transform, SimpleCollision>();
    for (auto entity : playerView) {
        auto &playerT = playerView.get<Transform>(entity);
        auto &player = playerView.get<Player>(entity);
        auto &playerCol = playerView.get<SimpleCollision>(entity);
        auto &playerHealth = playerView.get<Health>(entity);
        //auto &playerCollectables = playerView.get<Collectable>(entity);

        for(auto enemyEntity : enemyView)
        {
            auto &enemy = enemyView.get<DeadEndEnemy>(enemyEntity);
            auto &enemyT = enemyView.get<Transform>(enemyEntity);
            auto &enemyCol = enemyView.get<SimpleCollision>(enemyEntity);
            auto &enemyHealth = enemyView.get<Health>(enemyEntity);

            //enemy direction
            auto enemyX = enemyT.position.x;
            auto enemyY = enemyT.position.y;
            auto playerX = playerT.position.x;
            auto playerY = playerT.position.y;
            enemy.direction = {
                    (enemyX - playerX) / abs(enemyX - playerX),
                    (enemyY - playerY) / abs(enemyY - playerY),
                    0
            };

            //enemy collision
            if(enemyCol.colided) {
                for (auto bulletEntity : bulletView) {
                    auto &bullet = bulletView.get<Bullet>(bulletEntity);
                    auto &bulletCol = bulletView.get<SimpleCollision>(bulletEntity);
                    if (enemyView.contains(bulletCol.colidedWith))
                        enemyHealth.currentHealth -= bullet.damage;
                }

                if (Engine::Registry().valid(enemyCol.colidedWith)) {
                    SimpleCollision &collision = viewCollisions.get<SimpleCollision>(enemyCol.colidedWith);
                    Transform &transform = viewCollisions.get<Transform>(enemyCol.colidedWith);
                    Vector2 collisionSides = enemyCol.GetCollisionSides(enemyT.position, collision, transform.position);

                    do {
                        // get back for 1 frame
                        Float32 dt = Engine::DeltaTime();

                        if (collisionSides.x != 0) {
                            enemy.direction.x = 0;
                        }

                        if (collisionSides.y != 0) {
                            enemy.direction.y = 0;
                        }

                    } while (enemyCol.IsCollided(enemyT.position, collision, transform.position));

                }
            }

        }

    }
}