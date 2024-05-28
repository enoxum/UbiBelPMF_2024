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
#include "gameplay/common/simple_collisions.h"
#include <map>

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

	struct DamageEventEnemy
	{
		int ID;
		double damage;
	};

	struct DamageEventPlayer
	{
		double damage;
	};

	struct TileSystem
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
		double damage;
		Vector2 dir{ 0,0 };
	};
	
	struct MeleeWeaponSystem
	{
		bool isActive;
		bool isMouseBtnPressed;
		Vector2 position{ 0,0 };
	};

	struct MovementData
	{
		Float32 speed;
		Vector2 velocity{ 0.f, 0.f };
	};

	struct EnemyData
	{
		int ID;
		bool focusOnPlayer;
		bool wasHit;
		bool firstHit;
		float visionDistance;
		double damage;
		Vector3 target{ 0, 0, 0 };
	};

	struct Patrol {
		std::vector<Vector3> waypoints;
		int currentWaypointIndex;
		bool forward;
	};

	class PlayerController
		: public System
	{

	public:
		//static double playerSpeed;

		String SystemName() override {
			return "Character Controller System";
		}
		static std::unordered_map<int, Bullet*> bullets;
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
		OurEntity(const std::string& sprite_path, const std::string& animation_path, bool collidable, std::pair<int, int> collision_size) 
			: sprite_path_(sprite_path), animation_path_(animation_path), collidable_(collidable), collision_size_(collision_size)
		{
			instance = Engine::Registry().create();

			transform = &Engine::Registry().emplace<Transform>(instance);
			(*transform).position = Vector3{ 0.0f, 0.0f, 0.0f };

			sprite = &Engine::Registry().emplace<Sprite>(instance);
			AssignSprite(*sprite, sprite_path_);

			animator = &Engine::Registry().emplace<Animator>(instance);
			if (animation_path_ != "")
				AnimatorPlay(*animator, animation_path_);

			if (collidable_) {
				collision = &Engine::Registry().emplace<SimpleCollision>(instance);
				(*collision).size.x = collision_size_.second;
				(*collision).size.y = collision_size_.first;
			}
		}
		~OurEntity()
		{
			Engine::Instance().Registry().destroy(instance);
		}
		Entity instance;
		Transform* transform;
		Sprite* sprite;
		Animator* animator;
		SimpleCollision* collision;

		void move(Vector3 vector)
		{
			transform->position += vector;
		}
	private:
		std::string sprite_path_;
		std::string animation_path_;
		bool collidable_;
		std::pair<int, int> collision_size_;
	};

	//Character
	class Character :
		public OurEntity
	{
	public:
		Character(double hp, double speed, double strength, const std::string& sprite_path, const std::string& animation_path, bool collidable, std::pair<int, int> collision_size)
			: OurEntity(sprite_path, animation_path, collidable, collision_size), hp_(hp), speed_(speed), strength_(strength)
		{
			collidable_ = collidable;
			collision_size_ = collision_size;
		}
		virtual void spawn(const std::pair<int, int>&, const std::pair<int, int>&, const std::vector<std::vector<int>>&)
		{
		}
		virtual bool takeDamage(double damage)
		{
			return false;
		}
	private:
		virtual void die()
		{
		}
		virtual void collision()
		{
		}
	protected:
		double hp_;
		double speed_;
		double strength_;
		bool collidable_;
		std::pair<int, int> collision_size_;
	};

	//Enemy
	class Enemy :
		public Character
	{
	public:
		~Enemy()
		{

		}

		Enemy(int id)
			: Character(200.0, 40.0, 100.0, "souls_like_knight_character:IDLE:idle1", "souls_like_knight_character:IDLE", true, std::pair<int, int>(16, 32))
		{
			lootAmount_ = 100.0f;
			xpDrop_ = 100.0f;
			numOfWaypoints_ = (rand() % 4) + 3;

			movement_ = &Engine::Instance().Registry().emplace<MovementData>(instance);
			movement_->speed = speed_;

			data_ = &Engine::Instance().Registry().emplace<EnemyData>(instance);
			data_->ID = id;
			data_->focusOnPlayer = false;
			data_->wasHit = false;
			data_->firstHit = false;
			data_->damage = strength_;
			data_->visionDistance = 100.f;

			patrol_ = &Engine::Instance().Registry().emplace<Patrol>(instance);
			patrol_->currentWaypointIndex = 0;
			patrol_->forward = true;

			dmgEvent_ = &Engine::Registry().emplace<DamageEventEnemy>(instance);
			dmgEvent_->ID = data_->ID;
		}
		void spawn(const std::pair<int, int>& topLeft, const std::pair<int, int>& bottomRight, const std::vector<std::vector<int>>& matrix) override
		{
			int topLeftX = topLeft.second, topLeftY = topLeft.first;
			int	bottomRightX = bottomRight.second, bottomRightY = bottomRight.first;

			int randX = rand() % (bottomRightX - topLeftX) + topLeftX;
			int randY = rand() % (bottomRightY - topLeftY) + topLeftY;
			int tries = 0;
			while (matrix[randY][randX] != 0 && tries < 100)
			{
				randX = rand() % (bottomRightX - topLeftX) + topLeftX;
				randY = rand() % (bottomRightY - topLeftY) + topLeftY;
				tries++;
			}

			if (tries >= 100) {
				Logger::error("Something went really wrong! No possible spawnpoint for Enemy!");
				return;
			}

			initialPosition_ = Vector3{ randX * 64, -randY * 64, 0.0 };

			this->move(initialPosition_);

			generatePath(topLeft, bottomRight, matrix);
		}

		bool takeDamage(double damage) override
		{
			hp_ -= damage;
			if (hp_ <= 0.0)
			{
				die();
				return true;
			}
			return false;
		}

		EnemyData* data_;
		DamageEventEnemy* dmgEvent_;

	private:
		double xpDrop_;
		double lootAmount_;
		Vector3 initialPosition_;
		int numOfWaypoints_;
		MovementData* movement_;
		Patrol* patrol_;

		void generatePath(const std::pair<int, int>& topLeft, const std::pair<int, int>& bottomRight, const std::vector<std::vector<int>>& matrix)
		{
			int topLeftX = topLeft.second, topLeftY = topLeft.first;
			int	bottomRightX = bottomRight.second, bottomRightY = bottomRight.first;

			patrol_->waypoints.push_back(initialPosition_);
			Vector3 waypoint = initialPosition_;
			numOfWaypoints_--;

			while (numOfWaypoints_ > 0)
			{
				int randX = rand() % (bottomRightX - topLeftX) + topLeftX;
				int randY = rand() % (bottomRightY - topLeftY) + topLeftY;
				int tries = 0;
				while (matrix[randY][randX] != 0 && tries < 100)
				{
					randX = rand() % (bottomRightX - topLeftX) + topLeftX;
					randY = rand() % (bottomRightY - topLeftY) + topLeftY;
					tries++;
				}

				if (tries >= 100) {
					Logger::error("Something went really wrong! No possible position for waypoint!");
					return;
				}
				patrol_->waypoints.push_back(Vector3{ randX * 64, -randY * 64, 0.0 });

				numOfWaypoints_--;
			}
		}
		void die() override
		{
			delete this;
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
		Player()
			: Character(100.0, 100.0, 10.0, "BoberKuvar:beaver-SWEN", "", true, std::pair<int, int>(16, 32)), xp_(0), level_(1)
		{
			controller_ = &Engine::Instance().Registry().emplace<ControllerMapping>(instance);
			PlayerController::SetupPlayerInput(*controller_);

			sprite->size *= 0.7;

			movement_ = &Engine::Instance().Registry().emplace<MovementData>(instance);
			movement_->speed = speed_;

			Engine::Registry().emplace<common::CameraFollowFocus>(instance);

			dmg_ = &Engine::Instance().Registry().emplace<DamageEventPlayer>(instance);

			Engine::Dispatcher().sink<DamageEventPlayer>().connect<&Player::takeDamage>(this);
		}
		void spawn(const std::pair<int, int>& topLeft, const std::pair<int, int>& bottomRight, const std::vector<std::vector<int>>& matrix) override
		{
			int topLeftX = topLeft.second, topLeftY = topLeft.first;
			int	bottomRightX = bottomRight.second, bottomRightY = bottomRight.first;

			int randX = rand() % (bottomRightX - topLeftX) + topLeftX;
			int randY = rand() % (bottomRightY - topLeftY) + topLeftY;
			int tries = 0;
			while (matrix[randY][randX] != 0 && tries < 100)
			{
				randX = rand() % (bottomRightX - topLeftX) + topLeftX;
				randY = rand() % (bottomRightY - topLeftY) + topLeftY;
				tries++;
			}

			if (tries >= 100) {
				Logger::error("Something went really wrong! No possible spawnpoint for Bober!");
				return;
			}

			this->move(Vector3{ randX * 64, -randY * 64, 0.0 });
		}

		void takeDamage(DamageEventPlayer dmg)
		{
			hp_ -= dmg.damage;
			if (hp_ <= 0.0)
				die();
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
		DamageEventPlayer* dmg_;

	private:
		void die() override
		{
			for (auto weapon : weapons)
				delete weapon;
			delete this;
			exit(1);
		}
		void collision() override
		{

		}
		double xp_;
		int level_;
		ControllerMapping* controller_;
		MovementData* movement_;
	};

	//Weapon
	class Weapon :
		public OurEntity
	{
	public:
		~Weapon()
		{

		}
		Weapon(const std::string& sprite_path, double damage) : OurEntity(sprite_path, "", false, std::make_pair(0, 0)), damage_(damage)
		{

		}
	private:
		double damage_;
	};

	//Meele
	class Melee :
		public Weapon
	{
	public:
		Melee() : Weapon("pizzaSlice", 20.0)
		{
			melee_ = &Engine::Registry().emplace<MeleeWeaponSystem>(instance);
			melee_->isMouseBtnPressed = false;

			Engine::Registry().emplace<SlashEvent>(instance);
			Engine::Dispatcher().sink<SlashEvent>().connect<&Melee::slash>(this);
		}
		void slash(SlashEvent slash_)
		{

		}
	private:
		MeleeWeaponSystem* melee_;
		double cooldown_;
		double reach_;
	};

	//Bullet
	class Bullet :
		public OurEntity
	{
	public:
		Bullet(float speed, double damage) : OurEntity("pizzaSlice", "", true, std::make_pair(2,2)), speed_(speed), damage_(damage)
		{
			bullet_system = &Engine::Instance().Registry().emplace<BulletSystem>(instance);
			bullet_system->speed = speed;
			bullet_system->damage = damage;
		}
		~Bullet()
		{
			//delete bullet_system;
		}
		BulletSystem* bullet_system;
	private:
		float speed_;
		double damage_;
	};

	//Ranged
	class Ranged :
		public Weapon
	{
	public:
		Ranged(int currentAmmo, int magSize, double reloadSpeed) :Weapon("pizzaGun", 5.0), currentAmmo_(currentAmmo), magSize_(magSize), reloadSpeed_(reloadSpeed), numberOfBullets_(0)
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
				Bullet* bullet = new Bullet(shoot_.speed, 50.0);
				bullet->bullet_system->index = (++numberOfBullets_) % 100;
				PlayerController::bullets[bullet->bullet_system->index]=bullet;
				Vector2 scale(1, 1);
				constexpr float tileSize = 20.f;
				(*bullet->transform).position.x = shoot_.position.x;
				(*bullet->transform).position.y = shoot_.position.y;
				(*bullet->sprite).position.x = shoot_.position.x;
				(*bullet->sprite).position.y = shoot_.position.y;
				//AssignSprite(*bullet->sprite, "pizzaSlice");
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
		int numberOfBullets_;

		void reload()
		{
			currentAmmo_ = magSize_;
			return;
		};
	};

	//Tile
	class Tile :
		public OurEntity
	{
	public:
		Tile(const std::string& sprite_path, const std::string& animation_path, bool collidable)
			: OurEntity(sprite_path, animation_path, collidable, std::pair<int, int>(64, 64))
		{
			collidable_ = collidable;
			Engine::Registry().emplace<TileSystem>(instance);
		}
	private:
		bool collidable_;
	};

	//Room
	class Room
	{
	public:
		Room(int id, std::pair<int, int> top_left, std::pair<int, int> bottom_right, std::vector<std::pair<int, int>> doors_coords)
		{
			id_ = id;
			top_left_ = top_left;
			bottom_right_ = bottom_right;
			doors_coords_ = doors_coords;
			enemyCount_ = 2;
		}
		std::pair<int, int> getTopLeft()
		{
			return top_left_;
		}
		std::pair<int, int> getBottomRight()
		{
			return bottom_right_;
		}
		std::vector<std::pair<int, int>> getDoorsCoords()
		{
			return doors_coords_;
		}
		int getEnemyCount()
		{
			return enemyCount_;
		}
		std::unordered_map<int, Enemy*> getRoomEnemies()
		{
			return roomEnemies_;
		}
		void setRoomEnemies(std::unordered_map<int, Enemy*> roomEnemies)
		{
			roomEnemies_ = roomEnemies;
		}
	private:
		int id_;
		std::pair<int, int> top_left_;
		std::pair<int, int> bottom_right_;
		std::vector<std::pair<int, int>> doors_coords_;

		int enemyCount_;
		std::unordered_map<int, Enemy*> roomEnemies_;
		int roomType_; // za sada int
	};

	//Map
	class OurMap
	{
	public:
		OurMap(int n, int room_size)
		{
			n_ = n;
			room_size_ = room_size;
			matrix_ = std::vector<std::vector<int>>(n_, std::vector<int>(n_, 0));
			tile_matrix_ = std::vector<std::vector<Tile*>>(n_, std::vector<Tile*>(n_, 0));
			generateMap();

			Engine::Dispatcher().sink<DamageEventEnemy>().connect<&OurMap::findById>(this);
		}
		int get_n()
		{
			return n_;
		}
		std::vector<std::vector<int>> get_matrix() 
		{
			return matrix_;
		}
		std::vector<Room*> get_rooms() 
		{
			return rooms_;
		}
		void findById(DamageEventEnemy dmg)
		{
			for (Room* room : rooms_)
			{
				std::unordered_map<int, Enemy*> roomEnemies = room->getRoomEnemies();
				bool hasDied;
				for (auto elem : roomEnemies)
				{
					if (elem.first == dmg.ID)
					{
						hasDied = elem.second->takeDamage(dmg.damage);
						if (hasDied)
						{
							roomEnemies.erase(dmg.ID);
							break;
						}
					}
				}
				if(hasDied)
					room->setRoomEnemies(roomEnemies);
			}
		}

	private:
		int n_;
		int room_size_;
		std::vector<std::pair<int, int>> all_doors_coords_;
		std::vector<std::vector<int>> matrix_;
		std::vector<Room*> rooms_;
		std::vector<std::vector<Tile*>> tile_matrix_;

		void generateMap() 
		{
			make_wall_matrix();
			fill_rooms();
			rearange_interior();
			paint_map();
		}
		void make_wall_matrix() 
		{
			std::vector<std::vector<int>> horizontal_restriction_matrix(n_, std::vector<int>(n_, 0));
			std::vector<std::vector<int>> vertical_restriction_matrix(n_, std::vector<int>(n_, 0));

			for (int j = 0; j < n_; j++) {
				for (int i = 0; i < n_; i++) {
					if (i == 0) {
						matrix_[j][i] = 1;
						for (int k = 0; k < room_size_; k++)
							vertical_restriction_matrix[j][i + k] = 1;
					}
					if (j == 0) {
						matrix_[j][i] = 1;
						for (int k = 0; k < room_size_; k++)
							horizontal_restriction_matrix[j + k][i] = 1;
					}
					if (i == n_ - 1) {
						matrix_[j][i] = 1;
						for (int k = 0; k < room_size_; k++)
							vertical_restriction_matrix[j][i - k] = 1;
					}
					if (j == n_ - 1) {
						matrix_[j][i] = 1;
						for (int k = 0; k < room_size_; k++)
							horizontal_restriction_matrix[j - k][i] = 1;
					}
				}
			}

			int tries = 0;
			int max_tries = 10;
			while (tries < max_tries) {
				int direction = rand() % 2;
				int random_x = rand() % (n_ - 1) + 1;
				int random_y = rand() % (n_ - 1) + 1;

				//horizontal
				if (direction == 0) {
					if (matrix_[random_y][random_x] == 0 && horizontal_restriction_matrix[random_y][random_x] == 0) {
						tries = 0;

						//left
						int copy_x = random_x;
						while (matrix_[random_y][copy_x] == 0) {
							matrix_[random_y][copy_x] = 1;
							for (int k = 0; k < room_size_; k++) {
								horizontal_restriction_matrix[random_y + k][copy_x] = 1;
								horizontal_restriction_matrix[random_y - k][copy_x] = 1;
							}
							copy_x--;
						}
						for (int k = 0; k < room_size_; k++) {
							horizontal_restriction_matrix[random_y + k][copy_x] = 1;
							horizontal_restriction_matrix[random_y - k][copy_x] = 1;
						}
						int min_x = copy_x + 1;
						if (matrix_[random_y][copy_x] == 2) {
							matrix_[random_y][copy_x] = 1;
							matrix_[random_y - 1][copy_x] = 2;
							matrix_[random_y + 1][copy_x] = 2;
						}

						//right
						copy_x = random_x + 1;
						while (matrix_[random_y][copy_x] == 0) {
							matrix_[random_y][copy_x] = 1;
							for (int k = 0; k < room_size_; k++) {
								horizontal_restriction_matrix[random_y + k][copy_x] = 1;
								horizontal_restriction_matrix[random_y - k][copy_x] = 1;
							}
							copy_x++;
						}
						for (int k = 0; k < room_size_; k++) {
							horizontal_restriction_matrix[random_y + k][copy_x] = 1;
							horizontal_restriction_matrix[random_y - k][copy_x] = 1;
						}
						if (matrix_[random_y][copy_x] == 2) {
							matrix_[random_y][copy_x] = 1;
							matrix_[random_y - 1][copy_x] = 2;
							matrix_[random_y + 1][copy_x] = 2;
						}

						//door
						int max_x = copy_x - 1;
						int random_doorway_x = rand() % (max_x - min_x) + min_x;
						matrix_[random_y][random_doorway_x] = 2;
						all_doors_coords_.push_back(std::pair<int, int>(random_y, random_doorway_x));
					}
					else
						tries++;
				}
				//vertical
				else {
					if (matrix_[random_y][random_x] == 0 && vertical_restriction_matrix[random_y][random_x] == 0) {
						tries = 0;

						//up
						int copy_y = random_y;
						while (matrix_[copy_y][random_x] == 0) {
							matrix_[copy_y][random_x] = 1;
							for (int k = 0; k < room_size_; k++) {
								vertical_restriction_matrix[copy_y][random_x + k] = 1;
								vertical_restriction_matrix[copy_y][random_x - k] = 1;
							}
							copy_y--;
						}
						for (int k = 0; k < room_size_; k++) {
							vertical_restriction_matrix[copy_y][random_x + k] = 1;
							vertical_restriction_matrix[copy_y][random_x - k] = 1;
						}
						int min_y = copy_y + 1;
						if (matrix_[copy_y][random_x] == 2) {
							matrix_[copy_y][random_x] = 1;
							matrix_[copy_y][random_x - 1] = 2;
							matrix_[copy_y][random_x + 1] = 2;
						}

						//down
						copy_y = random_y + 1;
						while (matrix_[copy_y][random_x] == 0) {
							matrix_[copy_y][random_x] = 1;
							for (int k = 0; k < room_size_; k++) {
								vertical_restriction_matrix[copy_y][random_x + k] = 1;
								vertical_restriction_matrix[copy_y][random_x - k] = 1;
							}
							copy_y++;
						}
						for (int k = 0; k < room_size_; k++) {
							vertical_restriction_matrix[copy_y][random_x + k] = 1;
							vertical_restriction_matrix[copy_y][random_x - k] = 1;
						}

						//door
						int max_y = copy_y - 1;
						int random_doorway_y = rand() % (max_y - min_y) + min_y;
						matrix_[random_doorway_y][random_x] = 2;
						all_doors_coords_.push_back(std::pair<int, int>(random_doorway_y, random_x));
					}
					else
						tries++;
				}
			}

			print_matrix_in_console(matrix_);
			//print_matrix_in_console(horizontal_restriction_matrix);
			//print_matrix_in_console(vertical_restriction_matrix);
			//for (int i = 0; i < all_doors_coords_.size(); i++) {
			//	Logger::error(all_doors_coords_[i].second);
			//	Logger::error(all_doors_coords_[i].first);
			//	Logger::error("");
			//}
		}
		void fill_rooms()
		{
			int room_id = 0;
			for (int j = 1; j < n_ - 1; j++) {
				for (int i = 1; i < n_ - 1; i++) {
					int min_x = n_;
					int min_y = n_;
					int max_x = 0;
					int max_y = 0;
					std::vector<std::pair<int, int>> doors_coords;
					if (matrix_[j][i] == 0) {
						fill(i, j, &min_x, &min_y, &max_x, &max_y, &doors_coords);
						Room* room = new Room(room_id, std::pair<int, int>(min_y, min_x), std::pair<int, int>(max_y, max_x), doors_coords);
						room_id++;
						rooms_.push_back(room);
					}
				}
			}

			//print_matrix_in_console(matrix_);
		}
		void rearange_interior() 
		{
			for (auto room : rooms_) {
				std::pair<int, int> topLeft = (*room).getTopLeft();
				std::pair<int, int> bottomRight = (*room).getBottomRight();

				std::vector<std::pair<int, int>> doors_coords = (*room).getDoorsCoords();
				int door_number = doors_coords.size();

				std::pair<int, int> center = std::pair<int, int>((topLeft.first + bottomRight.first) / 2, (topLeft.second + bottomRight.second) / 2);
				std::vector<std::pair<int, int>> middles;
				for (int k = 0; k < door_number; k++) {
					std::pair<int, int> middle = std::pair<int, int>((center.first + doors_coords[k].first) / 2, (center.second + doors_coords[k].second) / 2);
					middles.push_back(middle);
				}

				for (int j = topLeft.first; j <= bottomRight.first; j++) {
					for (int i = topLeft.second; i <= bottomRight.second; i++) {
						for (int k = 0; k < door_number; k++) {
							int distance = calculate_distance(middles[k], std::pair<int, int>(j, i));
							int door_distance = calculate_distance(middles[k], doors_coords[k]);
							if (distance <= door_distance)
								matrix_[j][i] = 0;
							else if (matrix_[j][i] != 0)
								matrix_[j][i] = 1;
						}
					}
				}
			}
			//print_matrix_in_console(matrix_);

			// remove unnecessary walls
			for (int j = 1; j < n_ - 1; j++) {
				for (int i = 1; i < n_ - 1; i++) {
					if ((matrix_[j - 1][i] == 1 || matrix_[j - 1][i] == -1) &&
						(matrix_[j + 1][i] == 1 || matrix_[j + 1][i] == -1) &&
						(matrix_[j][i - 1] == 1 || matrix_[j][i - 1] == -1) &&
						(matrix_[j][i + 1] == 1 || matrix_[j][i + 1] == -1))
						matrix_[j][i] = -1;
				}
			}
			for (int j = 0; j < n_; j++) {
				if (matrix_[j][1] == 1 || matrix_[j][1] == -1)
					matrix_[j][0] = -1;
				if (matrix_[j][n_ - 2] == 1 || matrix_[j][n_ - 2] == -1)
					matrix_[j][n_ - 1] = -1;
			}
			for (int i = 0; i < n_; i++) {
				if (matrix_[1][i] == 1 || matrix_[1][i] == -1)
					matrix_[0][i] = -1;
				if (matrix_[n_ - 2][i] == 1 || matrix_[n_ - 2][i] == -1)
					matrix_[n_ - 1][i] = -1;
			}


			//print_matrix_in_console(matrix_);
		}
		void paint_map()
		{
			int sprite_size = 64;
			for (int j = 0; j < n_; j++) {
				for (int i = 0; i < n_; i++) {
					std::string tile_string;
					bool collidable = false;
					if (matrix_[j][i] == 0 || matrix_[j][i] == 2) {
						tile_string = "BoberKuvar:64x64 darkness";
						collidable = false;
					}
					else {
						tile_string = "BoberKuvar:64x64 backwall";
						collidable = true;
					}
					if (matrix_[j][i] == -1)
						continue;
					tile_matrix_[j][i] = new Tile(tile_string, "", collidable);
					tile_matrix_[j][i]->move(Vector3(sprite_size * i, -sprite_size * j, 0));
				}
			}

			//print_matrix_in_console(matrix_);
		}
		int calculate_distance(std::pair<int, int> a, std::pair<int, int> b)
		{
			return sqrt((a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second));
		}
		void print_matrix_in_console(std::vector<std::vector<int>> matrix)
		{
			for (int j = 0; j < n_; j++) {
				std::string row = "";
				for (int i = 0; i < n_; i++) {
					std::string number = std::to_string(matrix[j][i]);
					row.append(number);
					row.append(" ");
				}
				Logger::error(row);
			}
			Logger::error("\n");
		}
		void fill(int x, int y, int* min_x, int* min_y, int* max_x, int* max_y, std::vector<std::pair<int, int>>* doors_coords)
		{
			if (matrix_[y][x] == 0) {
				matrix_[y][x] = 1;
				*min_x = x < *min_x ? x : *min_x;
				*min_y = y < *min_y ? y : *min_y;
				*max_x = x > *max_x ? x : *max_x;
				*max_y = y > *max_y ? y : *max_y;
				fill(x - 1, y, min_x, min_y, max_x, max_y, doors_coords);
				fill(x + 1, y, min_x, min_y, max_x, max_y, doors_coords);
				fill(x, y - 1, min_x, min_y, max_x, max_y, doors_coords);
				fill(x, y + 1, min_x, min_y, max_x, max_y, doors_coords);
			}
			else if (matrix_[y][x] == 2) {
				int occurences = count((*doors_coords).begin(), (*doors_coords).end(), std::pair<int, int>(y, x));
				if (occurences == 0)
					(*doors_coords).push_back(std::pair<int, int>(y, x));
			}
		}
	};
}