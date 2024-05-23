#include "character_controller.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/game/transforms.h"
#include "core/system.h"
#include "esccape_controller.h"


using namespace dagger;
using namespace esccape;


// pomocne funkcije za okretanje spritea

void FaceSpriteRight(Sprite& sprite) {
	sprite.scale.x = std::abs(sprite.scale.x); // Ensure the sprite faces right
}

void FaceSpriteLeft(Sprite& sprite) {
	sprite.scale.x = -std::abs(sprite.scale.x); // Ensure the sprite faces left
}



void CharacterControllerFSM::Idle_Front::Enter(CharacterControllerFSM::StateComponent& state_)
{
	auto&& [animator, character] = Engine::Registry().get<Animator, EsccapeCharacter>(state_.entity);
	if(character.id == 0)
		AnimatorPlay(animator, "player:player_idle_front");
	else
		AnimatorPlay(animator, "skeleton:skeleton_idle_front");

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
	auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
	auto& character = Engine::Registry().get<EsccapeCharacter>(state_.entity);
	 
	FaceSpriteLeft(sprite);
	if (character.id == 0)
		AnimatorPlay(animator, "player:player_idle_right");
	else
		AnimatorPlay(animator, "skeleton:skeleton_idle_right");
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
	auto&& [animator, character] = Engine::Registry().get<Animator, EsccapeCharacter>(state_.entity);
	if (character.id == 0)
		AnimatorPlay(animator, "player:player_idle_back");
	else
		AnimatorPlay(animator, "skeleton:skeleton_idle_back");
}

void CharacterControllerFSM::Idle_Back::Run(CharacterControllerFSM::StateComponent& state_)
{
	auto& input = Engine::Registry().get<InputReceiver>(state_.entity);

	if (EPSILON_NOT_ZERO(input.Get("attack")))
	{
		GoTo(ECharacterStates::Attack_Up, state_);
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
	auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
	auto& character = Engine::Registry().get<EsccapeCharacter>(state_.entity);
	
	FaceSpriteRight(sprite);

	if (character.id == 0)
		AnimatorPlay(animator, "player:player_idle_right");
	else
		AnimatorPlay(animator, "skeleton:skeleton_idle_right");

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
	auto&& [animator, character] = Engine::Registry().get<Animator, EsccapeCharacter>(state_.entity);
	if (character.id == 0)
		AnimatorPlay(animator, "player:player_move_down");
	else
		AnimatorPlay(animator, "skeleton:skeleton_move_down");
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
		Vector2 newPosition = sprite.position;
		newPosition.y -= 50 * Engine::DeltaTime();

		if (!IsCollisionAt(newPosition)) {
			sprite.position.x = newPosition.x;
			sprite.position.y = newPosition.y;
		}
	}
}

void CharacterControllerFSM::Running_Down::Exit(CharacterControllerFSM::StateComponent& state_)
{
}

void CharacterControllerFSM::Running_Left::Enter(CharacterControllerFSM::StateComponent& state_)
{


	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
	auto& character = Engine::Registry().get<EsccapeCharacter>(state_.entity);

	FaceSpriteLeft(sprite);

	if (character.id == 0)
		AnimatorPlay(animator, "player:player_move_right");
	else
		AnimatorPlay(animator, "skeleton:skeleton_move_right");
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
		Vector2 newPosition = sprite.position;
		newPosition.x -= 50 * Engine::DeltaTime();

		if (!IsCollisionAt(newPosition)) {
			sprite.position.x = newPosition.x;
			sprite.position.y = newPosition.y;
		}
	}
}

void CharacterControllerFSM::Running_Left::Exit(CharacterControllerFSM::StateComponent& state_)
{
}

void CharacterControllerFSM::Running_Right::Enter(CharacterControllerFSM::StateComponent& state_)
{

	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
	auto& character = Engine::Registry().get<EsccapeCharacter>(state_.entity);

	FaceSpriteRight(sprite);

	if (character.id == 0)
		AnimatorPlay(animator, "player:player_move_right");
	else
		AnimatorPlay(animator, "skeleton:skeleton_move_right");
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
		Vector2 newPosition = sprite.position;
		newPosition.x += 50 * Engine::DeltaTime();

		if (!IsCollisionAt(newPosition)) {
			sprite.position.x = newPosition.x;
			sprite.position.y = newPosition.y;
		}
	}
}

void CharacterControllerFSM::Running_Right::Exit(CharacterControllerFSM::StateComponent& state_)
{
}

void CharacterControllerFSM::Running_Up::Enter(CharacterControllerFSM::StateComponent& state_)
{
	auto&& [animator, character] = Engine::Registry().get<Animator, EsccapeCharacter>(state_.entity);
	if (character.id == 0)
		AnimatorPlay(animator, "player:player_move_up");
	else
		AnimatorPlay(animator, "skeleton:skeleton_move_up");
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
		Vector2 newPosition = sprite.position;
		newPosition.y += 50 * Engine::DeltaTime();

		if (!IsCollisionAt(newPosition)) {
			sprite.position.x = newPosition.x;
			sprite.position.y = newPosition.y;
		}
	}
}


void CharacterControllerFSM::Running_Up::Exit(CharacterControllerFSM::StateComponent& state_)
{
}

void CharacterControllerFSM::Attack_Down::Enter(CharacterControllerFSM::StateComponent& state_)
{

	auto&& [animator, character] = Engine::Registry().get<Animator, EsccapeCharacter>(state_.entity);
	if (character.id == 0)
		AnimatorPlay(animator, "player:player_attack_down");
	else
		AnimatorPlay(animator, "skeleton:skeleton_attack_down");
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
	auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
	auto& character = Engine::Registry().get<EsccapeCharacter>(state_.entity);

	FaceSpriteLeft(sprite);

	if (character.id == 0)
		AnimatorPlay(animator, "player:player_attack_right");
	else
		AnimatorPlay(animator, "skeleton:skeleton_attack_right");
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
	auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
	auto& character = Engine::Registry().get<EsccapeCharacter>(state_.entity);

	FaceSpriteRight(sprite);

	if (character.id == 0)
		AnimatorPlay(animator, "player:player_attack_right");
	else
		AnimatorPlay(animator, "skeleton:skeleton_attack_right");
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

	auto&& [animator, character] = Engine::Registry().get<Animator, EsccapeCharacter>(state_.entity);
	if (character.id == 0)
		AnimatorPlay(animator, "player:player_attack_up");
	else
		AnimatorPlay(animator, "skeleton:skeleton_attack_up");
}

void CharacterControllerFSM::Attack_Up::Run(CharacterControllerFSM::StateComponent& state_)
{
	auto& input = Engine::Registry().get<InputReceiver>(state_.entity);
	if (EPSILON_ZERO(input.Get("attack")))
	{
		GoTo(ECharacterStates::Idle_Back, state_);
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

	auto&& [animator, character] = Engine::Registry().get<Animator, EsccapeCharacter>(state_.entity);
	if (character.id == 0)
		AnimatorPlay(animator, "player:player_death");
	else
		AnimatorPlay(animator, "skeleton:skeleton_death");
}

void CharacterControllerFSM::Death::Run(CharacterControllerFSM::StateComponent& state_)
{

	auto&& [animator, character] = Engine::Registry().get<Animator, EsccapeCharacter>(state_.entity);
	if (character.id == 0)
		AnimatorPlay(animator, "player:player_death");
	else
		AnimatorPlay(animator, "skeleton:skeleton_death");
}

void CharacterControllerFSM::Death::Exit(CharacterControllerFSM::StateComponent& state_)
{
}


bool IsCollisionAt(Vector2 position)
{
	return false;
}
