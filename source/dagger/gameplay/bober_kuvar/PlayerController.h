#pragma once

#include "core/core.h"
#include "core/system.h"
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

using namespace dagger;

namespace bober_game
{
	class OurEntity;
	class PlayerController;
	class Character;
	class Player;
	class Enemy;
	class Weapon;
	class Ranged;
	class Meele;
	class Bullet;
	class OurMap;
	class Room;
	class Tile;

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
		int ttl = 1000;
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
		static std::vector<Bullet*> bullets;
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

	//Entity
	class OurEntity
	{
	public:
		OurEntity()
		{
			instance = Engine::Instance().Registry().create();

			transform = &Engine::Instance().Registry().emplace<Transform>(instance);
			(*transform).position = Vector3{ 0.0f, 0.0f, 0.0f };

			sprite = &Engine::Instance().Registry().emplace<Sprite>(instance);

			animator = &Engine::Instance().Registry().emplace<Animator>(instance);
		}
		~OurEntity()
		{
			/*delete transform;
			delete sprite;
			delete animator;*/
			Engine::Instance().Registry().destroy(instance);
		}
		Entity instance;
		Transform* transform;
		Sprite* sprite;
		Animator* animator;
	private:
	};

	//Character
	class Character :
		public OurEntity
	{
	public:
		Character() : OurEntity() 
		{

		}
		void move(Vector3 vector)
		{
			transform->position += vector;
		}
	private:
		virtual void spawn()
		{

		}
		virtual void die()
		{

		}
		virtual void collision()
		{

		}

		double hp;
		double speed;
		double strength;
	};

	//Enemy
	class Enemy :
		public Character
	{
	public:
		Enemy() : Character()
		{
			AssignSprite(*sprite, "souls_like_knight_character:IDLE:idle1");
			AnimatorPlay(*animator, "souls_like_knight_character:IDLE");
			lootAmount = 100.0f;
			xpDrop = 100.0f;
		}
	private:
		double xpDrop;
		double lootAmount;

		void spawn() override
		{

		}
		void die() override
		{

		}
		void collision() override
		{

		}
	};

	//Player
	class Player :
		public Character
	{
	public:
		Player() : Character()
		{
			controller_ = &Engine::Instance().Registry().emplace<ControllerMapping>(instance);
			PlayerController::SetupPlayerInput(*controller_);
			AssignSprite(*sprite, "souls_like_knight_character:IDLE:idle1");
			AnimatorPlay(*animator, "souls_like_knight_character:IDLE");
			Engine::Registry().emplace<common::CameraFollowFocus>(instance);
		}

		void levelUp()
		{
			level_++;
		}
		int getLevel()
		{
			return level_;
		}
		std::vector<Weapon*> weapons;
	private:
		void spawn() override
		{
			//weapons.push_back(new Melee());
		}
		void die() override
		{

		}
		void collision() override
		{

		}
		void shoot()
		{

		}

		double xp_;
		int level_;
		ControllerMapping* controller_;
	};

	//Weapon
	class Weapon :
		public OurEntity
	{
	public:
		Weapon(double damage) : OurEntity()
		{
			damage_ = damage;
		}
	private:
		double damage_;
	};

	//Meele
	class Melee :
		public Weapon
	{
	public:
		Melee() : Weapon(20.0)
		{
			melee = &Engine::Registry().emplace<MeleeWeaponSystem>(instance);
			melee->isMouseBtnPressed = false;

			Engine::Registry().emplace<SlashEvent>(instance);
			Engine::Dispatcher().sink<SlashEvent>().connect<&Melee::slash>(this);
		}
		void slash(SlashEvent slash_)
		{

		}
	private:
		MeleeWeaponSystem* melee;
		double cooldown;
		double reach;
	};

	//Bullet
	class Bullet :
		public OurEntity
	{
	public:
		Bullet(float speed) : OurEntity(), speed_(speed)
		{
			bullet_system = &Engine::Instance().Registry().emplace<BulletSystem>(instance);
			bullet_system->speed = speed;
		}
		~Bullet()
		{
			//delete bullet_system;
		}
		BulletSystem* bullet_system;
	private:
		float speed_;
	};

	//Ranged
	class Ranged :
		public Weapon
	{
	public:
		Ranged(int currentAmmo, int magSize, double reloadSpeed) :Weapon(5.0), currentAmmo_(currentAmmo), magSize_(magSize), reloadSpeed_(reloadSpeed)
		{
			ranged = &Engine::Registry().emplace<RangedWeaponSystem>(instance);
			ranged->isMouseBtnPressed = false;

			Engine::Registry().emplace<ShootEvent>(instance);
			Engine::Dispatcher().sink<ShootEvent>().connect<&Ranged::shoot>(this);
			Engine::Dispatcher().sink<ReloadEvent>().connect<&Ranged::reload>(this);
		}
		void shoot(ShootEvent shoot_)
		{
			if (currentAmmo_ != 0) {
				Bullet* bullet = new Bullet(shoot_.speed);
				bullet->bullet_system->index = PlayerController::bullets.size();
				PlayerController::bullets.push_back(bullet);
				Vector2 scale(1, 1);
				constexpr float tileSize = 20.f;
				(*bullet->transform).position.x = shoot_.position.x;
				(*bullet->transform).position.y = shoot_.position.y;
				(*bullet->sprite).position.x = shoot_.position.x;
				(*bullet->sprite).position.y = shoot_.position.y;
				AssignSprite(*bullet->sprite, "pizzaSlice");
				(*bullet->sprite).size = scale * tileSize;
				Engine::Registry().remove<Animator>(bullet->instance);
				currentAmmo_--;
			}
		};
	private:
		RangedWeaponSystem* ranged;
		int currentAmmo_;
		int magSize_;
		double reloadSpeed_;

		void reload()
		{
			currentAmmo_ = magSize_;
			return;
		};
	};

	//Map
	class OurMap
	{
	private:
		std::list<Room> rooms_;

		void generateMap()
		{

		}
	};

	//Room
	class Room
	{
	private:
		int enemyCount_;
		std::list<Enemy> roomEnemies_;
		int roomType_; // za sada int

		void generateRoom()
		{

		}
	};

	//Tile
	class Tile :
		public OurEntity
	{
	private:
		bool collidable_;
	};
}