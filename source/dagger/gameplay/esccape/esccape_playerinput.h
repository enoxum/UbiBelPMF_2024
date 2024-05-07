#pragma once

#include "core/system.h"
#include "core/core.h"

#include "esccape_player.h"

using namespace dagger;

namespace esccape
{
    struct ControllerMapping
    {
        EDaggerKeyboard up_key;
        EDaggerKeyboard down_key;
        EDaggerKeyboard left_key;
        EDaggerKeyboard right_key;

        Vector4 input{ 0, 0, 0, 0 };
    };

    class EsccapePlayerInputSystem
        : public System
    {
        static Float32 s_BoarderRadius;

    public:
        static Float32 s_PlayerSpeed;
        static EscPlayer s_Player;

    public:
        inline String SystemName() { return "Esccape Player Input System"; }

        void SpinUp() override;
        void WindDown() override;
        void Run() override;

        static void SetupPlayerInput(ControllerMapping& controllerMapping_)
        {
            controllerMapping_.up_key = EDaggerKeyboard::KeyW;
            controllerMapping_.down_key = EDaggerKeyboard::KeyS;
            controllerMapping_.left_key = EDaggerKeyboard::KeyA;
            controllerMapping_.right_key = EDaggerKeyboard::KeyD;
        }

        static void SetupPlayerBoarders(Float32 boarderRadius_)
        {
            s_BoarderRadius = boarderRadius_;
        }

    private:

        void OnKeyboardEvent(KeyboardEvent kEvent_);
    };
}