#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace bober_game
{
	struct ControllerMapping
	{
		EDaggerKeyboard up_key;
		EDaggerKeyboard left_key;
		EDaggerKeyboard down_key;
		EDaggerKeyboard right_key;

		Vector2 input = { 0.f, 0.f };
	};

	class PlayerController
		: public System
	{

	public:
		static Float32 player_speed;

		String SystemName() override {
			return "Character Controller System";
		}

		void SpinUp() override;
		void WindDown() override;
		void Run() override;

		static void SetupPlayerInput(ControllerMapping& controllerMapping_)
		{
			controllerMapping_.up_key = EDaggerKeyboard::KeyW;
			controllerMapping_.left_key = EDaggerKeyboard::KeyA;
			controllerMapping_.down_key = EDaggerKeyboard::KeyS;
			controllerMapping_.right_key = EDaggerKeyboard::KeyD;
		}
	private:

		void OnKeyboardEvent(KeyboardEvent kEvent_);
	};
}