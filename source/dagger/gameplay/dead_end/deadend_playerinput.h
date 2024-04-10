#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace dead_end
{
	struct ControllerMapping
	{
		// moving
		EDaggerKeyboard up_key;
		EDaggerKeyboard down_key;
		EDaggerKeyboard left_key;
		EDaggerKeyboard right_key;

		Vector2 input{ 0,0 };

		// shooting
		EDaggerKeyboard shoot_key;
		bool shooting = { false };
		
	};

	class DeadEndPlayerInputSystem
		: public System
	{
	public:
		inline String SystemName() { return "DeadEnd Player Input System"; }

		void SpinUp() override;
		void WindDown() override;
		void Run() override;

		static void SetupPlayerInput(ControllerMapping& controllerMapping_)
		{
			controllerMapping_.up_key = EDaggerKeyboard::KeyW;
			controllerMapping_.down_key = EDaggerKeyboard::KeyS;
			controllerMapping_.left_key = EDaggerKeyboard::KeyA;
			controllerMapping_.right_key = EDaggerKeyboard::KeyD;

			controllerMapping_.shoot_key = EDaggerKeyboard::KeySpace;
		}

	private:

		void OnKeyboardEvent(KeyboardEvent kEvent_);
	};
}

