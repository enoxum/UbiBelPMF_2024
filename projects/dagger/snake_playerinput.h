#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace red_snake
{
    struct SnakeControllerMapping
    {
        EDaggerKeyboard up_key;
        EDaggerKeyboard down_key;
        EDaggerKeyboard left_key;
        EDaggerKeyboard right_key;

        Vector2 input{ 0, 0 };
    };

    class SnakePlayerInputSystem : public System
    {
    public:
        inline String SystemName() { return "Snake Player Input System"; }

        void SpinUp() override;
        void WindDown() override;
        void Run() override;

        static void SetupPlayerInput(SnakeControllerMapping& controllerMapping_)
        {
            controllerMapping_.up_key = EDaggerKeyboard::KeyW;
            controllerMapping_.down_key = EDaggerKeyboard::KeyS;
            controllerMapping_.left_key = EDaggerKeyboard::KeyA;
            controllerMapping_.right_key = EDaggerKeyboard::KeyD;
        }

        static void SetSnakeSpeed(Float32 speed_)
        {
            s_SnakeSpeed = speed_;
        }



    private:
        void OnKeyboardEvent(KeyboardEvent kEvent_);
        static Float32 s_SnakeSpeed;
    };
}
