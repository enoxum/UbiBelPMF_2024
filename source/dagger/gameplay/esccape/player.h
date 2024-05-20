#pragma once

#include "core/system.h"
#include "core/core.h"
#include "character_controller.h"



using namespace dagger; 

namespace esccape 
{
    struct ControllerMapping
    {
        EDaggerKeyboard leftKey = EDaggerKeyboard::KeyA;
        EDaggerKeyboard rightKey = EDaggerKeyboard::KeyD;
        EDaggerKeyboard upKey = EDaggerKeyboard::KeyW;
        EDaggerKeyboard downKey = EDaggerKeyboard::KeyS;
        EDaggerKeyboard attackKey = EDaggerKeyboard::KeySpace;

        Vector2 input{ 0, 0 };
    };

    struct PlayerEntity
    {
        float speed;
        int health;
    };

    class Player
        : public System
    {
    public:
        Player();
        Player(PlayerEntity playerEntity, std::function<void(int)> healthChangedCallback);
        inline String SystemName() { return "Player Input System"; }

        void SpinUp() override;
        void WindDown() override;
        void Run() override;
        float getSpeed();
        void setSpeed(float speed);
        int getHealth();
        void setHealth(int health);
        void setHealthChangedCallback(std::function<void(int)> callback);

    private:
        PlayerEntity player;
        std::function<void(int)> healthChangedCallback;

        void OnKeyboardEvent(KeyboardEvent kEvent_);
        
    };
}


