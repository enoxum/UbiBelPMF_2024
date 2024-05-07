#pragma once
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

class OurEntity
{
public:
	OurEntity();
	Entity instance;
	Transform* transform;
	Sprite* sprite;
	Animator* animator;
private:
};