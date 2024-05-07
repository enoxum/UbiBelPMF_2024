#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger; 

namespace esccape 
{
    struct ControllerMapping
    {
        EDaggerKeyboard leftKey = EDaggerKeyboard::KeyA;
        EDaggerKeyboard rightKey = EDaggerKeyboard::KeyD;
        EDaggerKeyboard upKey = EDaggerKeyboard::KeyW;
        EDaggerKeyboard downKey = EDaggerKeyboard::KeyS;

        Vector2 input{ 0, 0 };
    };

    struct PlayerEntity
    {
        Float32 speed;
    };

    class Player
        : public System
    {
    public:
        inline String SystemName() { return "Player Input System"; }

        void SpinUp() override;
        void WindDown() override;
        void Run() override;

    private:

        void OnKeyboardEvent(KeyboardEvent kEvent_);
    };
}


