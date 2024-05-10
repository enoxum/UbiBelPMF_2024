#include "deadend_player.h"

#include "core/engine.h"
#include "core/core.h"
#include "core/game/transforms.h"
#include "core/graphics/text.h"

#include "gameplay/common/simple_collisions.h"

#include <algorithm>

using namespace dagger;
using namespace dead_end;

void dead_end::PlayerSystem::Run()
{
	auto viewCollisions = Engine::Registry().view<Transform, SimpleCollision>();
	auto view = Engine::Registry().view<Player, Transform, SimpleCollision>();
	//auto viewText = Engine::Registry().view<Text>();

	for (auto entity : view)
	{
		auto& t = view.get<Transform>(entity);
		auto& player = view.get<Player>(entity);
		auto& col = view.get<SimpleCollision>(entity);

		//auto& text = viewText.get<Text>(entity); add gameover later.

		if (col.colided)
		{
            // later : collision w/ enemies and collectables.
            
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
            }

            col.colided = false;
			
		}
	}
}
