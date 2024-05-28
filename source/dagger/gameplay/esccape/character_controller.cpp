#include "character_controller.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/game/transforms.h"
#include "core/system.h"
#include "esccape_controller.h"

#include "character.h"
#include "bullet.h"
#include "boost.h"
#include "blackboard_manager.h"
#include "worm.h"

using namespace dagger;
using namespace esccape;

BlackboardManager& CharacterControllerFSM::blackboardManager = BlackboardManager::GetInstance();

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
	if (character.id == 0)
		AnimatorPlay(animator, "player:player_idle_front");
	else
		AnimatorPlay(animator, "skeleton:skeleton_idle_front");

}

void CharacterControllerFSM::Idle_Front::Run(CharacterControllerFSM::StateComponent& state_)
{
	auto& input = Engine::Registry().get<InputReceiver>(state_.entity);
	auto& character = Engine::Registry().get<EsccapeCharacter>(state_.entity);
	if (character.health <= 0)
	{
		GoTo(ECharacterStates::Death, state_);
	}
	if (!character.healthSystem.IsAlive())
	{
		GoTo(ECharacterStates::Death, state_);
	}
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
	auto& character = Engine::Registry().get<EsccapeCharacter>(state_.entity);
	if (character.health <= 0)
	{
		GoTo(ECharacterStates::Death, state_);
	}
	if (!character.healthSystem.IsAlive())
	{
		GoTo(ECharacterStates::Death, state_);
	}
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
	auto& character = Engine::Registry().get<EsccapeCharacter>(state_.entity);
	if (character.health <= 0)
	{
		GoTo(ECharacterStates::Death, state_);
	}
	if (!character.healthSystem.IsAlive())
	{
		GoTo(ECharacterStates::Death, state_);
	}
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
	auto& character = Engine::Registry().get<EsccapeCharacter>(state_.entity);
	if (character.health <= 0)
	{
		GoTo(ECharacterStates::Death, state_);
	}
	if (!character.healthSystem.IsAlive())
	{
		GoTo(ECharacterStates::Death, state_);
	}
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
	auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
	auto& transform = Engine::Registry().get<Transform>(state_.entity);
	auto& character = Engine::Registry().get<EsccapeCharacter>(state_.entity);

	if (EPSILON_ZERO(input.Get("down")))
	{
		GoTo(ECharacterStates::Idle_Front, state_);
	}
	else
	{
		auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
		sprite.position.y -= character.speed * Engine::DeltaTime();
		transform.position.y = sprite.position.y;
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
	auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
	auto& transform = Engine::Registry().get<Transform>(state_.entity);
	auto& character = Engine::Registry().get<EsccapeCharacter>(state_.entity);

	if (EPSILON_ZERO(input.Get("left")))
	{
		GoTo(ECharacterStates::Idle_Left, state_);
	}
	else
	{
		auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
		sprite.position.x -= character.speed * Engine::DeltaTime();
		transform.position.x = sprite.position.x; 
		if (character.health <= 0)
			GoTo(ECharacterStates::Death, state_);
		if(!character.healthSystem.IsAlive())
			GoTo(ECharacterStates::Death, state_);

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
	auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
	auto& transform = Engine::Registry().get<Transform>(state_.entity);
	auto& character = Engine::Registry().get<EsccapeCharacter>(state_.entity);

	if (EPSILON_ZERO(input.Get("right")))
	{
		GoTo(ECharacterStates::Idle_Right, state_);
	}
	else
	{
		auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
		sprite.position.x += character.speed * Engine::DeltaTime();
		transform.position.x = sprite.position.x;
		if (character.health <= 0)
			GoTo(ECharacterStates::Death, state_);
		if (!character.healthSystem.IsAlive())
			GoTo(ECharacterStates::Death, state_);
		
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
	auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
	auto& transform = Engine::Registry().get<Transform>(state_.entity);
	auto& character = Engine::Registry().get<EsccapeCharacter>(state_.entity);

	if (EPSILON_ZERO(input.Get("up")))
	{
		GoTo(ECharacterStates::Idle_Back, state_);
	}
	else
	{
		auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
		sprite.position.y += character.speed * Engine::DeltaTime();
		transform.position.y = sprite.position.y;
		if (character.health <= 0)
			GoTo(ECharacterStates::Death, state_);
		if (!character.healthSystem.IsAlive())
			GoTo(ECharacterStates::Death, state_);
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
	auto&& [input, character] = Engine::Registry().get<InputReceiver, EsccapeCharacter>(state_.entity);

	if (character.attackCooldown == 0)
	{
		character.attackCooldown = 300;

		if (EPSILON_NOT_ZERO(input.Get("right")))
		{
			GoTo(ECharacterStates::Running_Right, state_);
		}
		else if (EPSILON_NOT_ZERO(input.Get("left")))
		{
			GoTo(ECharacterStates::Running_Left, state_);
		}
		else if (EPSILON_NOT_ZERO(input.Get("up")))
		{
			GoTo(ECharacterStates::Running_Up, state_);
		}
		else if (EPSILON_NOT_ZERO(input.Get("down")))
		{
			GoTo(ECharacterStates::Running_Down, state_);
		}
		else
		{
			GoTo(ECharacterStates::Idle_Front, state_);
		}

	}
	else
	{
		character.attackCooldown--;
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
	auto&& [input, character] = Engine::Registry().get<InputReceiver, EsccapeCharacter>(state_.entity);

	if (character.attackCooldown == 0)
	{
		character.attackCooldown = 300;

		if (EPSILON_NOT_ZERO(input.Get("right")))
		{
			GoTo(ECharacterStates::Running_Right, state_);
		}
		else if (EPSILON_NOT_ZERO(input.Get("left")))
		{
			GoTo(ECharacterStates::Running_Left, state_);
		}
		else if (EPSILON_NOT_ZERO(input.Get("up")))
		{
			GoTo(ECharacterStates::Running_Up, state_);
		}
		else if (EPSILON_NOT_ZERO(input.Get("down")))
		{
			GoTo(ECharacterStates::Running_Down, state_);
		}
		else
		{
			GoTo(ECharacterStates::Idle_Left, state_);
		}

	}
	else
	{
		character.attackCooldown--;
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
	auto&& [input, character] = Engine::Registry().get<InputReceiver, EsccapeCharacter>(state_.entity);

	if (character.attackCooldown == 0)
	{
		character.attackCooldown = 300;

		if (EPSILON_NOT_ZERO(input.Get("right")))
		{
			GoTo(ECharacterStates::Running_Right, state_);
		}
		else if (EPSILON_NOT_ZERO(input.Get("left")))
		{
			GoTo(ECharacterStates::Running_Left, state_);
		}
		else if (EPSILON_NOT_ZERO(input.Get("up")))
		{
			GoTo(ECharacterStates::Running_Up, state_);
		}
		else if (EPSILON_NOT_ZERO(input.Get("down")))
		{
			GoTo(ECharacterStates::Running_Down, state_);
		}
		else
		{
			GoTo(ECharacterStates::Idle_Right, state_);
		}

	}
	else
	{
		character.attackCooldown--;
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
	auto&& [input, character] = Engine::Registry().get<InputReceiver, EsccapeCharacter>(state_.entity);

	if (character.attackCooldown == 0)
	{
		character.attackCooldown = 300;

		if (EPSILON_NOT_ZERO(input.Get("right")))
		{
			GoTo(ECharacterStates::Running_Right, state_);
		}
		else if (EPSILON_NOT_ZERO(input.Get("left")))
		{
			GoTo(ECharacterStates::Running_Left, state_);
		}
		else if (EPSILON_NOT_ZERO(input.Get("up")))
		{
			GoTo(ECharacterStates::Running_Up, state_);
		}
		else if (EPSILON_NOT_ZERO(input.Get("down")))
		{
			GoTo(ECharacterStates::Running_Down, state_);
		}
		else
		{
			GoTo(ECharacterStates::Idle_Back, state_);
		}

	}
	else
	{
		character.attackCooldown--;
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
	auto&& [input, character] = Engine::Registry().get<InputReceiver, EsccapeCharacter>(state_.entity);

	if (character.attackCooldown == 0)
	{
		character.attackCooldown = 300;
		GoTo(ECharacterStates::Dead, state_);

	}
	else
	{
		character.attackCooldown--;
	}

}

void CharacterControllerFSM::Death::Exit(CharacterControllerFSM::StateComponent& state_)
{


}

void CharacterControllerFSM::Dead::Enter(CharacterControllerFSM::StateComponent& state_)
{

	auto&& [animator, character] = Engine::Registry().get<Animator, EsccapeCharacter>(state_.entity);
	if (character.id == 0)
		AnimatorPlay(animator, "player:player_dead");
	else
		AnimatorPlay(animator, "skeleton:skeleton_dead");
}

void CharacterControllerFSM::Dead::Run(CharacterControllerFSM::StateComponent& state_)
{
}

void CharacterControllerFSM::Dead::Exit(CharacterControllerFSM::StateComponent& state_)
{


}

std::pair<Entity, Entity> CheckCollisionsFSM(CharacterControllerFSM::StateComponent& state_, BlackboardManager& bbManager)
{
	auto& transform = Engine::Registry().get<Transform>(state_.entity);
	auto& collision = Engine::Registry().get<SimpleCollision>(state_.entity);
	
	auto viewCollisions = Engine::Registry().view<Transform, SimpleCollision>();

	for (auto otherEntity : viewCollisions) {
		if (otherEntity == state_.entity) continue;

		auto& otherCollision = viewCollisions.get<SimpleCollision>(otherEntity);
		auto& otherTransform = viewCollisions.get<Transform>(otherEntity);

		auto attackCollisionFirst = collision;
		attackCollisionFirst.size *= 1.5;
		
		auto attackCollisionSecond = otherCollision;
		attackCollisionSecond.size *= 1.5;

		if (attackCollisionFirst.IsCollided(transform.position, attackCollisionSecond, otherTransform.position)) {

			auto& input = Engine::Registry().get<InputReceiver>(state_.entity);
			if (EPSILON_NOT_ZERO(input.Get("attack"))) {
				bool isCharacter = Engine::Registry().has<EsccapeCharacter>(otherEntity);
				if (isCharacter) {
					auto& chr = Engine::Registry().get<EsccapeCharacter>(state_.entity);
					auto& otherCharacter = Engine::Registry().get<EsccapeCharacter>(otherEntity);

					otherCharacter.health -= 0.01f;
					otherCharacter.healthSystem.TakeDamage(0.01);


					printf("Character %d health = %f\n",(int)otherCharacter.id, otherCharacter.healthSystem.GetCurrentHealth());
				}
			}
		}

		if (collision.IsCollided(transform.position, otherCollision, otherTransform.position)) {
			bbManager.SetCollided(state_.entity, true);
			bbManager.SetCollided(otherEntity, true);
			bbManager.RecordCollision(state_.entity, otherEntity);

			return std::make_pair(state_.entity, otherEntity);
		}
		
	}

	bbManager.SetCollided(state_.entity, false);
	return std::make_pair(entt::null, entt::null);
}


void ResolveCollision(Entity entity1, Entity collidedWith, BlackboardManager bbManager)
{
	if (!Engine::Registry().valid(collidedWith)) return;

	auto& transform = Engine::Registry().get<Transform>(entity1);
	auto& collision = Engine::Registry().get<SimpleCollision>(entity1);
	auto& otherTransform = Engine::Registry().get<Transform>(collidedWith);
	auto& otherCollision = Engine::Registry().get<SimpleCollision>(collidedWith);
	bool otherIsCharacter = Engine::Registry().has<Character>(collidedWith);
	bool otherIsBullet = Engine::Registry().has<Bullet>(collidedWith);
	bool otherIsBoost = Engine::Registry().has<Boost>(collidedWith);
	bool otherIsWorm = Engine::Registry().has<Worm>(collidedWith);

	
	Vector2 collisionSides = collision.GetCollisionSides(transform.position, otherCollision, otherTransform.position);

	while (collision.IsCollided(transform.position, otherCollision, otherTransform.position)) {
		if (otherIsBullet) {
			auto& character = Engine::Registry().get<EsccapeCharacter>(entity1);
			character.healthSystem.TakeDamage(1.0f);
			character.healthSystem.TakeDamage(1.0f);
			character.healthSystem.TakeDamage(0.05f);
			Engine::Registry().destroy(collidedWith);

			break;
		}

		if (otherIsWorm) {
			auto& character = Engine::Registry().get<EsccapeCharacter>(entity1);
			character.healthSystem.TakeDamage(20.0f);
			collision.size = { 0, 0 };
			

			break;
		}

		if (otherIsBoost) {
			auto& boost = Engine::Registry().get<Boost>(collidedWith);
			auto& character = Engine::Registry().get<EsccapeCharacter>(entity1);
			if (boost.id == 0) {
				character.healthSystem.Heal(2.0f);
				Engine::Registry().destroy(collidedWith);
				printf("Character %d health = %f\n", (int)character.id, character.healthSystem.GetCurrentHealth());

				break;
			}
			else if(boost.id == 1){
				String inputContext;
				auto& input = Engine::Registry().get<InputReceiver>(entity1);
				if (character.id == 0 && !character.inputContextReversed)
				{
					inputContext = "ASDWSpaceReverse";
					character.inputContextReversed = true;
				}
				else if (character.id == 0 && character.inputContextReversed)
				{
					inputContext = "ASDWSpace";
					character.inputContextReversed = false;
				}
				else if (character.id == 1 && !character.inputContextReversed)
				{
					inputContext = "skeleton-arrows-reverse";
					character.inputContextReversed = true;
				}
				else if (character.id == 1 && character.inputContextReversed)
				{
					inputContext = "skeleton-arrows";
					character.inputContextReversed = false;
				}
				input.contexts.pop_back();
				input.contexts.push_back(inputContext);
				printf("Character %d controls switched!\n", character.id);
				Engine::Registry().destroy(collidedWith);
				break;
			}
			else if(boost.id == 2){

				character.speed *= 1.1;
				printf("Character %d speed changed: %f\n", character.id, character.speed);
				Engine::Registry().destroy(collidedWith);
				break;
			}
			else if (boost.id == 3) {

				character.speed *= 0.9;
				printf("Character %d speed changed: %f\n", character.id, character.speed);
				Engine::Registry().destroy(collidedWith);
				break;
			}
		}

		if (otherIsCharacter)
		{
			Vector2 otherCollisionSides = otherCollision.GetCollisionSides(otherTransform.position, collision, transform.position);

			if (std::abs(collisionSides.x) > 0 || std::abs(otherCollisionSides.x) > 0) {
				float moveAmountX = (transform.position.x < otherTransform.position.x) ? -0.5f : 0.5f;
				transform.position.x += moveAmountX;
				otherTransform.position.x -= moveAmountX;

				collisionSides = collision.GetCollisionSides(transform.position, otherCollision, otherTransform.position);
				otherCollisionSides = otherCollision.GetCollisionSides(otherTransform.position, collision, transform.position);
				
			}

			if (std::abs(collisionSides.y) > 0 || std::abs(otherCollisionSides.y) > 0) {
				float moveAmountY = (transform.position.y < otherTransform.position.y) ? -0.5f : 0.5f;
				transform.position.y += moveAmountY;
				otherTransform.position.y -= moveAmountY;
			}

			break;
		}

		if (std::abs(collisionSides.x) > 0 && collision.IsCollided(transform.position, otherCollision, otherTransform.position)) {
			
			transform.position.x -= transform.position.x < 0 ? -3 : 3;
		}
		if (std::abs(collisionSides.y) > 0 && collision.IsCollided(transform.position, otherCollision, otherTransform.position)) {
			transform.position.y -= transform.position.y < 0 ? -3 : 3;
		}
	}

	bbManager.SetCollided(entity1, false);
}
