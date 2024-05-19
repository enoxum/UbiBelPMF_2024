#pragma once

#include "core/core.h"
#include "core/system.h"
#include "OurEntity.h"

using namespace dagger;

namespace bober_game
{
	struct ControllerMapping
	{
		EDaggerKeyboard upKey;
		EDaggerKeyboard leftKey;
		EDaggerKeyboard downKey;
		EDaggerKeyboard rightKey;

		Vector2 input = { 0.f, 0.f };
	};

	struct Cursor
	{
		bool isMouseBtnPressed = false;
		Vector2 position{ 0, 0 };
	};

	class PlayerController
		: public System
	{

	public:
		static double playerSpeed;

		String SystemName() override {
			return "Character Controller System";
		}

		void SpinUp() override;
		void WindDown() override;
		void Run() override;
		void OnCursorMoveEvent(CursorEvent cursor_);
		void OnMouseEvent(MouseEvent input_);

		static void SetupPlayerInput(ControllerMapping& controllerMapping_)
		{
			controllerMapping_.upKey = EDaggerKeyboard::KeyW;
			controllerMapping_.leftKey = EDaggerKeyboard::KeyA;
			controllerMapping_.downKey = EDaggerKeyboard::KeyS;
			controllerMapping_.rightKey = EDaggerKeyboard::KeyD;
		}
	private:
		void OnKeyboardEvent(KeyboardEvent kEvent_);
	};
}