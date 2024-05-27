/*#pragma once
#include "core/core.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/audio.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite_render.h"
#include "core/graphics/textures.h"
#include "core/graphics/animations.h"
#include "core/graphics/gui.h"
#include "core/graphics/text.h"
#include "tools/diagnostics.h"
#include "gameplay/common/parallax.h"
#include "gameplay/common/camera_focus.h"
#include <gameplay/common/simple_collisions.h>

class OurEntity
{
public:
	OurEntity(const std::string &sprite_path, const std::string &animation_path, bool collidable, std::pair<int, int> collision_size);
  ~OurEntity();
	Entity instance;
	Transform* transform;
	Sprite* sprite;
	Animator* animator;
	SimpleCollision* collision;

	void move(Vector3 vector);
private:
	std::string sprite_path_;
	std::string animation_path_;
	bool collidable_;
	std::pair<int, int> collision_size_;
};
*/
