#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace pacman
{
    struct EnemyData
    {
        int ID;
        Vector3 target{ 0, 0, 0 };
    };

    struct MovementData
    {
        Float32 maxSpeed;
        Float32 acceleration;
        bool isFrictionOn{ false };
        Vector2 velocity{ 0, 0 };
    };

    struct ControllerMapping
    {
        EDaggerKeyboard up_key;
        EDaggerKeyboard down_key;
        EDaggerKeyboard left_key;
        EDaggerKeyboard right_key;

        Vector2 input{ 0, 0 };
    };

    class PacmanPlayerInputSystem
        : public System
    {
        static Float32 s_BoarderUp;
        static Float32 s_BoarderDown;

    public:

        static Float32 s_PlayerSpeed;

    public:
        inline String SystemName() { return "Pacman Player Input System"; }

        void SpinUp() override;
        void WindDown() override;
        void Run() override;

#if !defined(NDEBUG)
        void RenderToolMenu();
        void RenderToolWindow();
#endif //!defined(NDEBUG)

        static void SetupPlayerOneInput(ControllerMapping& controllerMapping_)
        {
            controllerMapping_.up_key = EDaggerKeyboard::KeyW;
            controllerMapping_.down_key = EDaggerKeyboard::KeyS;
            controllerMapping_.left_key = EDaggerKeyboard::KeyA;
            controllerMapping_.right_key = EDaggerKeyboard::KeyD;
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