#pragma once
#include "core/core.h"
#include "core/game/finite_state_machine.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"

#include "blackboard_manager.h"
#include <ctime>


enum struct ECharacterStates
{
	Idle_Front,
	Idle_Back,
	Idle_Right,
	Idle_Left,
	Running_Right,
	Running_Left,
	Running_Up,
	Running_Down,
	Attack_Right,
	Attack_Left,
	Attack_Up,
	Attack_Down,
	Death,
	Dead
};


struct CharacterControllerFSM : public FSM<ECharacterStates>
{
	DEFINE_STATE(CharacterControllerFSM, ECharacterStates, Idle_Front);
	DEFINE_STATE(CharacterControllerFSM, ECharacterStates, Idle_Back);
	DEFINE_STATE(CharacterControllerFSM, ECharacterStates, Idle_Right);
	DEFINE_STATE(CharacterControllerFSM, ECharacterStates, Idle_Left);
	DEFINE_STATE(CharacterControllerFSM, ECharacterStates, Running_Right);
	DEFINE_STATE(CharacterControllerFSM, ECharacterStates, Running_Left);
	DEFINE_STATE(CharacterControllerFSM, ECharacterStates, Running_Up);
	DEFINE_STATE(CharacterControllerFSM, ECharacterStates, Running_Down);
	DEFINE_STATE(CharacterControllerFSM, ECharacterStates, Attack_Right);
	DEFINE_STATE(CharacterControllerFSM, ECharacterStates, Attack_Left);
	DEFINE_STATE(CharacterControllerFSM, ECharacterStates, Attack_Up);
	DEFINE_STATE(CharacterControllerFSM, ECharacterStates, Attack_Down);
	DEFINE_STATE(CharacterControllerFSM, ECharacterStates, Death);
	DEFINE_STATE(CharacterControllerFSM, ECharacterStates, Dead);

	CharacterControllerFSM()
	{
		CONNECT_STATE(ECharacterStates, Idle_Front);
		CONNECT_STATE(ECharacterStates, Idle_Back);
		CONNECT_STATE(ECharacterStates, Idle_Right);
		CONNECT_STATE(ECharacterStates, Idle_Left);
		CONNECT_STATE(ECharacterStates, Running_Right);
		CONNECT_STATE(ECharacterStates, Running_Left);
		CONNECT_STATE(ECharacterStates, Running_Up);
		CONNECT_STATE(ECharacterStates, Running_Down);
		CONNECT_STATE(ECharacterStates, Attack_Right);
		CONNECT_STATE(ECharacterStates, Attack_Left);
		CONNECT_STATE(ECharacterStates, Attack_Up);
		CONNECT_STATE(ECharacterStates, Attack_Down);
		CONNECT_STATE(ECharacterStates, Death);
		CONNECT_STATE(ECharacterStates, Dead);
	}

	static BlackboardManager& blackboardManager;
};

std::pair<Entity, Entity> CheckCollisionsFSM(CharacterControllerFSM::StateComponent& state_, BlackboardManager& bbManager);
void ResolveCollision(Entity e1, Entity collidedWith, BlackboardManager bbManager);
