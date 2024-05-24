
#include "core/system.h"
#include "core/core.h"
#include <string>
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/game/transforms.h"
#include "gameplay/common/simple_collisions.h"
#include "esccape_controller.h"
#include "character_controller.h"

using namespace dagger;

namespace esccape
{
    class Character 
    {
    public:
        Character(Entity entity, Sprite* sprite, Animator* animator, InputReceiver* input, esccape::EsccapeCharacter* character, Transform* transform, SimpleCollision* collision);
        static Character Get(Entity entity);

        static Character Create(
            const std::string& input_ = "",
            const std::string& spritesheet_ = "",
            const std::string& animation_ = "",
            const ColorRGB& color_ = { 1, 1, 1 },
            const Vector2& position_ = { 0, 0 },
            int id = 0
        );

        Entity getEntity() const;
        Sprite* getSprite();
        Animator* getAnimator();
        InputReceiver* getInputReceiver();
        esccape::EsccapeCharacter* getEsccapeCharacter();


        void CheckCollisions();

    private:
        Entity entity;
        Sprite* sprite;
        Animator* animator;
        InputReceiver* input;
        esccape::EsccapeCharacter* character;
        SimpleCollision* collision;
        Transform* transform;
    };
}



////////////////////*****************************************//////////////////////////////////////



//#pragma once
//
//#include "core/system.h"
//#include "core/core.h"
//#include "character_controller.h"
//
//
//
//using namespace dagger; 
//
//namespace esccape 
//{
//    struct ControllerMapping
//    {
//        EDaggerKeyboard leftKey = EDaggerKeyboard::KeyA;
//        EDaggerKeyboard rightKey = EDaggerKeyboard::KeyD;
//        EDaggerKeyboard upKey = EDaggerKeyboard::KeyW;
//        EDaggerKeyboard downKey = EDaggerKeyboard::KeyS;
//        EDaggerKeyboard attackKey = EDaggerKeyboard::KeySpace;
//
//        Vector2 input{ 0, 0 };
//    };
//
//    struct PlayerEntity
//    {
//        float speed;
//        int health;
//    };
//
//    class Player
//        : public System
//    {
//    public:
//        Player();
//        Player(PlayerEntity playerEntity, std::function<void(int)> healthChangedCallback);
//        inline String SystemName() { return "Player Input System"; }
//
//        void SpinUp() override;
//        void WindDown() override;
//        void Run() override;
//        float getSpeed();
//        void setSpeed(float speed);
//        int getHealth();
//        void setHealth(int health);
//        void setHealthChangedCallback(std::function<void(int)> callback);
//
//    private:
//        PlayerEntity player;
//        std::function<void(int)> healthChangedCallback;
//
//        void OnKeyboardEvent(KeyboardEvent kEvent_);
//        
//    };
//}
//
