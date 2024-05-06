#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace ping_pong
{

    struct  MovementData
    {
        Float32 maxSpeed{0.01};
        Float32 acceleration{0};
        Float32 angle {0.0};
        Float32 radius {200.0};
    };
    struct ControllerMapping
    {
        EDaggerKeyboard up_key;
        EDaggerKeyboard down_key;
        EDaggerKeyboard left_key;
        EDaggerKeyboard right_key;
        EDaggerKeyboard eye_shoot;

        Vector2 input{ 0, 0 };
        Vector2 doubleTap{0, 0};
        bool eyeShoot{false};
        double last_clicked_left;
        double last_clicked_right;
    };

    struct StatsData{
        float hp{100};
    };

    class PingPongPlayerInputSystem
        : public System
    {
        static Float32 s_BoarderUp;
        static Float32 s_BoarderDown;

    public:

        static Float32 s_PlayerSpeed;

    public:
        inline String SystemName() { return "PingPong Player Input System"; }

        void SpinUp() override;
        void WindDown() override;
        void Run() override;

        static void SetupPlayerOneInput(ControllerMapping& controllerMapping_)
        {
            controllerMapping_.up_key = EDaggerKeyboard::KeyW;
            controllerMapping_.down_key = EDaggerKeyboard::KeyS;
            controllerMapping_.left_key = EDaggerKeyboard::KeyA;
            controllerMapping_.right_key = EDaggerKeyboard::KeyD;
            controllerMapping_.eye_shoot = EDaggerKeyboard::KeySpace;
        }

        static void SetupPlayerTwoInput(ControllerMapping& controllerMapping_)
        {
            controllerMapping_.up_key = EDaggerKeyboard::KeyUp;
            controllerMapping_.down_key = EDaggerKeyboard::KeyDown;
        }

        static void SetupPlayerBoarders(Float32 boarderUp_, Float32 boarderDown_)
        {
            s_BoarderUp = boarderUp_;
            s_BoarderDown = boarderDown_;
        }

    private:

        void OnKeyboardEvent(KeyboardEvent kEvent_);
    };
}