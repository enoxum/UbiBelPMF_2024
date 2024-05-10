#include "deadend_camera.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"
#include "core/graphics/window.h"

#include "core/game/transforms.h"

using namespace dagger;
using namespace dead_end;

void dead_end::DeadEndCameraFollowSystem::Run()
{

	auto* camera = Engine::GetDefaultResource<Camera>();

	auto view = Engine::Registry().view<Transform, DeadEndCamera>();

	for (auto entity : view)
	{
		auto& t = view.get<Transform>(entity);
		Vector2 center{ t.position.x, t.position.y };
		camera->position = Vector3{ glm::mix((Vector2)camera->position, center, 0.5f), 0.0f };
	}
}
