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
#include "deadend_bullet.h"

using namespace dagger;
using namespace dead_end;

void dead_end::PlayerSystem::Run()
{
	auto viewCollisions = Engine::Registry().view<Transform, SimpleCollision>();
	auto view = Engine::Registry().view<Player, Transform, SimpleCollision>();
    auto viewBullet = Engine::Registry().view<Bullet, SimpleCollision>();
	//auto viewText = Engine::Registry().view<Text>();

    WaveSystem waveSystem;
    waveSystem.Initialize();
    int waveNumber = 1; // start wave

	for (auto entity : view)
	{
		auto& t = view.get<Transform>(entity);
		auto& player = view.get<Player>(entity);
		auto& col = view.get<SimpleCollision>(entity);
        auto& health = view.get<Health>(entity);
        auto& collectables = view.get<Collectable>(entity);

        

		//auto& text = viewText.get<Text>(entity); add gameover later.
        waveSystem.SetWave(waveNumber);
        waveSystem.UpdateWave(player, health);


		if (col.colided)
		{
            // later : collision w/ enemies and collectables.

            // collectables collision
            if (Engine::Registry().has<Collectable>(col.colidedWith)) {
                Collectable& collectable = Engine::Registry().get<Collectable>(col.colidedWith);
                CollectableSystem collectableSystem;
                collectableSystem.HandleCollectableCollision(player, collectable);
                Engine::Registry().destroy(col.colidedWith); // Uništavanje kolekcionara nakon prikupljanja
            }

            if (viewBullet.contains(col.colidedWith)) {
                col.colided = false;
                continue;
            }
            
			if (Engine::Registry().valid(col.colidedWith))
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

                // Decrease health upon collision
                player.health--;
                if (player.health <= 0)
                {
                    player.health = 0;
                    player.stopMoving = true; // Stop movement if health is zero
                }

            }

            col.colided = false;
			
		}
	}
}
