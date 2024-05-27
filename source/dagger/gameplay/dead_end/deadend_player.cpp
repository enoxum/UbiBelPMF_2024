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
#include <iostream>
#include "deadend_bullet.h"
#include "deadend_enemy.h"
#include "deadend_obstacle.h"

using namespace dagger;
using namespace dead_end;

void dead_end::PlayerSystem::Run()
{
	auto view = Engine::Registry().view<Player, Transform, SimpleCollision, Health>();
	auto viewCollisions = Engine::Registry().view<DeadEndObstacle, Transform, SimpleCollision>();
    auto viewBullet = Engine::Registry().view<Bullet, SimpleCollision>();
    auto viewCollect = Engine::Registry().view<Collectable>();
    auto viewEnemy = Engine::Registry().view<DeadEndEnemy, SimpleCollision>();
	//auto viewText = Engine::Registry().view<Text>();


	for (auto entity : view)
	{
		auto& t = view.get<Transform>(entity);
		auto& player = view.get<Player>(entity);
		auto& col = view.get<SimpleCollision>(entity);
        auto& health = view.get<Health>(entity);

		//auto& text = viewText.get<Text>(entity); add gameover later.

       

		if (col.colided)
		{
            // later : collision w/ enemies and collectables.

            if (Engine::Registry().valid(col.colidedWith) && viewEnemy.contains(col.colidedWith))
            {
                auto& enemy = viewEnemy.get<DeadEndEnemy>(col.colidedWith);

                player.hit.first= true;
                player.hit.second = enemy.meleeDamage;
                handleHit(player, health);
                col.colided = false;
                break;
            }

            // collectables collision
            if (viewCollect.contains(col.colidedWith)) {
                Collectable& collectable = Engine::Registry().get<Collectable>(col.colidedWith);
                dead_end::HandleCollectableCollision(collectable, health);
                Engine::Registry().destroy(col.colidedWith);
                continue;
            }

            if (viewBullet.contains(col.colidedWith)) {
                col.colided = false;
                continue;
            }

            
			if (Engine::Registry().valid(col.colidedWith) && viewCollisions.contains(col.colidedWith))
			{
				SimpleCollision& collision = viewCollisions.get<SimpleCollision>(col.colidedWith);
				Transform& transform = viewCollisions.get<Transform>(col.colidedWith);

				Vector2 collisionSides = col.GetCollisionSides(t.position, collision, transform.position);

                do
                {
                    // get back for 1 frame 
                    Float32 dt = Engine::DeltaTime();

                    if (collisionSides.x > 0)
                    {
                        t.position.x -= player.speed * dt;
                    }
                    else if (collisionSides.x < 0)
                    {
                        t.position.x += player.speed * dt;
                    }

                    if (collisionSides.y > 0)
                    {
                        t.position.y -= player.speed * dt;
                    }
                    else if (collisionSides.y < 0)
                    {
                        t.position.y += player.speed * dt;
                    }

                } while (col.IsCollided(t.position, collision, transform.position));


            }

            col.colided = false;
			
		}
	}
}

void dead_end::PlayerSystem::handleHit(Player& player_, Health& health_)
{
    auto value = player_.hit.second;
    health_.currentHealth -= value;
    std::cout << "Curr HP : " << health_.currentHealth << std::endl;

    if (health_.currentHealth <= 0)
    {
        player_.stopMoving = true;
        // handle game over.
    }
}

