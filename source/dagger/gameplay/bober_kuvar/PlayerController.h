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
		EDaggerKeyboard num1;
		EDaggerKeyboard num2;
		EDaggerKeyboard reloadKey;

		Vector2 input = { 0.f, 0.f };
		int index=0;
	};

	struct ReloadEvent 
	{
	};

	struct Cursor
	{
		bool isMouseBtnPressed = false;
		Vector2 position{ 0, 0 };
	};

	struct ShootEvent 
	{
		float speed;
		Vector2 position{ 0,0 };
	};

	struct SlashEvent	
	{
		float range;
	};

	struct RangedWeaponSystem 
	{
		bool isActive;
		bool isMouseBtnPressed;
		Vector2 position{ 0,0 };
	};

	struct BulletSystem 
	{
		int index;
		bool firstTime = true;
		float speed;
		Vector2 dir{ 0,0 };
		int ttl = 5000;
	};
	
	struct MeleeWeaponSystem
	{
		bool isActive;
		bool isMouseBtnPressed;
		Vector2 position{ 0,0 };
	};

	class PlayerController
		: public System
	{

	public:
		static double playerSpeed;

		String SystemName() override {
			return "Character Controller System";
		}
		//static std::vector<Bullet*> bullets;
		void SpinUp() override;
		void WindDown() override;
		void Run() override;
		void OnCursorMoveEvent(CursorEvent cursor_);
		void OnMouseEvent(MouseEvent input_);
		void OnShoot(ShootEvent shoot_);
		void OnSlash(SlashEvent slash_);

		static void SetupPlayerInput(ControllerMapping& controllerMapping_)
		{
			controllerMapping_.upKey = EDaggerKeyboard::KeyW;
			controllerMapping_.leftKey = EDaggerKeyboard::KeyA;
			controllerMapping_.downKey = EDaggerKeyboard::KeyS;
			controllerMapping_.rightKey = EDaggerKeyboard::KeyD;
			controllerMapping_.num1 = EDaggerKeyboard::Key1;
			controllerMapping_.num2 = EDaggerKeyboard::Key2;
			controllerMapping_.reloadKey = EDaggerKeyboard::KeyR;
		}
	private:
		void OnKeyboardEvent(KeyboardEvent kEvent_);
	};
}