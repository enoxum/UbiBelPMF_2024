#include "character_controller.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/game/transforms.h"
#include "core/system.h"


using namespace dagger;



void CharacterControllerFSM::Idle_Front::Enter(CharacterControllerFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "player:player_idle_front");
}

void CharacterControllerFSM::Idle_Front::Run(CharacterControllerFSM::StateComponent& state_)
{
	auto& input = Engine::Registry().get<InputReceiver>(state_.entity);

	if (EPSILON_NOT_ZERO(input.Get("attack")))
	{
		GoTo(ECharacterStates::Attack_Down, state_);
	}
	else if (EPSILON_NOT_ZERO(input.Get("up")))
	{
		GoTo(ECharacterStates::Running_Up, state_);
	}
	else if (EPSILON_NOT_ZERO(input.Get("down")))
	{
		GoTo(ECharacterStates::Running_Down, state_);
	}
	else if (EPSILON_NOT_ZERO(input.Get("left")))
	{
		GoTo(ECharacterStates::Running_Left, state_);
	}
	else if (EPSILON_NOT_ZERO(input.Get("right")))
	{
		GoTo(ECharacterStates::Running_Right, state_);
	}
}

void CharacterControllerFSM::Idle_Front::Exit(CharacterControllerFSM::StateComponent& state_)
{
}

void CharacterControllerFSM::Idle_Left::Enter(CharacterControllerFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "player:player_idle_left");
}

void CharacterControllerFSM::Idle_Left::Run(CharacterControllerFSM::StateComponent& state_)
{
	auto& input = Engine::Registry().get<InputReceiver>(state_.entity);

	if (EPSILON_NOT_ZERO(input.Get("attack")))
	{
		GoTo(ECharacterStates::Attack_Left, state_);
	}
	else if (EPSILON_NOT_ZERO(input.Get("up")))
	{
		GoTo(ECharacterStates::Running_Up, state_);
	}
	else if (EPSILON_NOT_ZERO(input.Get("down")))
	{
		GoTo(ECharacterStates::Running_Down, state_);
	}
	else if (EPSILON_NOT_ZERO(input.Get("left")))
	{
		GoTo(ECharacterStates::Running_Left, state_);
	}
	else if (EPSILON_NOT_ZERO(input.Get("right")))
	{
		GoTo(ECharacterStates::Running_Right, state_);
	}
}

void CharacterControllerFSM::Idle_Left::Exit(CharacterControllerFSM::StateComponent& state_)
{
}

void CharacterControllerFSM::Idle_Back::Enter(CharacterControllerFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "player:player_idle_back");
}

void CharacterControllerFSM::Idle_Back::Run(CharacterControllerFSM::StateComponent& state_)
{
	auto& input = Engine::Registry().get<InputReceiver>(state_.entity);

	if (EPSILON_NOT_ZERO(input.Get("attack")))
	{
		GoTo(ECharacterStates::Attack_Left, state_);
	}
	else if (EPSILON_NOT_ZERO(input.Get("up")))
	{
		GoTo(ECharacterStates::Running_Up, state_);
	}
	else if (EPSILON_NOT_ZERO(input.Get("down")))
	{
		GoTo(ECharacterStates::Running_Down, state_);
	}
	else if (EPSILON_NOT_ZERO(input.Get("left")))
	{
		GoTo(ECharacterStates::Running_Left, state_);
	}
	else if (EPSILON_NOT_ZERO(input.Get("right")))
	{
		GoTo(ECharacterStates::Running_Right, state_);
	}
}

void CharacterControllerFSM::Idle_Back::Exit(CharacterControllerFSM::StateComponent& state_)
{
}

void CharacterControllerFSM::Idle_Right::Enter(CharacterControllerFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "player:player_idle_right");
}

void CharacterControllerFSM::Idle_Right::Run(CharacterControllerFSM::StateComponent& state_)
{
	auto& input = Engine::Registry().get<InputReceiver>(state_.entity);

	if (EPSILON_NOT_ZERO(input.Get("attack")))
	{
		GoTo(ECharacterStates::Attack_Right, state_);
	}
	else if (EPSILON_NOT_ZERO(input.Get("up")))
	{
		GoTo(ECharacterStates::Running_Up, state_);
	}
	else if (EPSILON_NOT_ZERO(input.Get("down")))
	{
		GoTo(ECharacterStates::Running_Down, state_);
	}
	else if (EPSILON_NOT_ZERO(input.Get("left")))
	{
		GoTo(ECharacterStates::Running_Left, state_);
	}
	else if (EPSILON_NOT_ZERO(input.Get("right")))
	{
		GoTo(ECharacterStates::Running_Right, state_);
	}
}

void CharacterControllerFSM::Idle_Right::Exit(CharacterControllerFSM::StateComponent& state_)
{
}


void CharacterControllerFSM::Running_Down::Enter(CharacterControllerFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "player:player_move_down");
}

void CharacterControllerFSM::Running_Down::Run(CharacterControllerFSM::StateComponent& state_)
{
	auto& input = Engine::Registry().get<InputReceiver>(state_.entity);
	if (EPSILON_ZERO(input.Get("down")))
	{
		GoTo(ECharacterStates::Idle_Front, state_);
	}
	else
	{
		auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
		sprite.position.y -= Engine::DeltaTime();
	}
}

void CharacterControllerFSM::Running_Down::Exit(CharacterControllerFSM::StateComponent& state_)
{
}

void CharacterControllerFSM::Running_Left::Enter(CharacterControllerFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "player:player_move_left");
}

void CharacterControllerFSM::Running_Left::Run(CharacterControllerFSM::StateComponent& state_)
{
	auto& input = Engine::Registry().get<InputReceiver>(state_.entity);
	if (EPSILON_ZERO(input.Get("left")))
	{
		GoTo(ECharacterStates::Idle_Left, state_);
	}
	else
	{
		auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
		sprite.position.x -= Engine::DeltaTime();
	}
}

void CharacterControllerFSM::Running_Left::Exit(CharacterControllerFSM::StateComponent& state_)
{
}

void CharacterControllerFSM::Running_Right::Enter(CharacterControllerFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "player:player_move_right");
}

void CharacterControllerFSM::Running_Right::Run(CharacterControllerFSM::StateComponent& state_)
{
	auto& input = Engine::Registry().get<InputReceiver>(state_.entity);
	if (EPSILON_ZERO(input.Get("right")))
	{
		GoTo(ECharacterStates::Idle_Right, state_);
	}
	else
	{
		auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
		sprite.position.x += Engine::DeltaTime();
	}
}

void CharacterControllerFSM::Running_Right::Exit(CharacterControllerFSM::StateComponent& state_)
{
}

void CharacterControllerFSM::Running_Up::Enter(CharacterControllerFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "player:player_move_up");
}

void CharacterControllerFSM::Running_Up::Run(CharacterControllerFSM::StateComponent& state_)
{
	auto& input = Engine::Registry().get<InputReceiver>(state_.entity);
	if (EPSILON_ZERO(input.Get("up")))
	{
		GoTo(ECharacterStates::Idle_Back, state_);
	}
	else
	{
		auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
		sprite.position.y += Engine::DeltaTime();
	}
}


void CharacterControllerFSM::Running_Up::Exit(CharacterControllerFSM::StateComponent& state_)
{
}

void CharacterControllerFSM::Attack_Down::Enter(CharacterControllerFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "player:player_attack_down");
}

void CharacterControllerFSM::Attack_Down::Run(CharacterControllerFSM::StateComponent& state_)
{
	auto& input = Engine::Registry().get<InputReceiver>(state_.entity);
	if (EPSILON_ZERO(input.Get("attack")))
	{
		GoTo(ECharacterStates::Idle_Front, state_);
	}
	else
	{
		auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
		sprite.position.y -= Engine::DeltaTime();
	}

}

void CharacterControllerFSM::Attack_Down::Exit(CharacterControllerFSM::StateComponent& state_)
{
}

void CharacterControllerFSM::Attack_Left::Enter(CharacterControllerFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "player:player_attack_left");
}

void CharacterControllerFSM::Attack_Left::Run(CharacterControllerFSM::StateComponent& state_)
{
	auto& input = Engine::Registry().get<InputReceiver>(state_.entity);
	if (EPSILON_ZERO(input.Get("attack")))
	{
		GoTo(ECharacterStates::Idle_Left, state_);
	}
	else
	{
		auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
		sprite.position.y -= Engine::DeltaTime();
	}
}

void CharacterControllerFSM::Attack_Left::Exit(CharacterControllerFSM::StateComponent& state_)
{
}

void CharacterControllerFSM::Attack_Right::Enter(CharacterControllerFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "player:player_attack_right");
}

void CharacterControllerFSM::Attack_Right::Run(CharacterControllerFSM::StateComponent& state_)
{
	auto& input = Engine::Registry().get<InputReceiver>(state_.entity);
	if (EPSILON_ZERO(input.Get("attack")))
	{
		GoTo(ECharacterStates::Idle_Right, state_);
	}
	else
	{
		auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
		sprite.position.y -= Engine::DeltaTime();
	}

}

void CharacterControllerFSM::Attack_Right::Exit(CharacterControllerFSM::StateComponent& state_)
{
}

void CharacterControllerFSM::Attack_Up::Enter(CharacterControllerFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "player:player_attack_up");
}

void CharacterControllerFSM::Attack_Up::Run(CharacterControllerFSM::StateComponent& state_)
{
	auto& input = Engine::Registry().get<InputReceiver>(state_.entity);
	if (EPSILON_ZERO(input.Get("attack")))
	{
		GoTo(ECharacterStates::Idle_Right, state_);
	}
	else
	{
		auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
		sprite.position.y -=  Engine::DeltaTime();
	}
}


void CharacterControllerFSM::Attack_Up::Exit(CharacterControllerFSM::StateComponent& state_)
{
}

void CharacterControllerFSM::Death::Enter(CharacterControllerFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "player:player_death");
}

void CharacterControllerFSM::Death::Run(CharacterControllerFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "player:player_death");
}

void CharacterControllerFSM::Death::Exit(CharacterControllerFSM::StateComponent& state_)
{
}

