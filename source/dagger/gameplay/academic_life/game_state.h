#pragma once
#include "core/graphics/text.h"
#include "core/system.h"
#include "core/core.h"
#include "core/engine.h"
#include "gameplay/common/particles.h"
#include "core/graphics/sprite.h"

namespace academic_life {

    struct GameStateControllerMapping {

        EDaggerKeyboard rightKey = EDaggerKeyboard::KeyD;
        EDaggerKeyboard leftKey = EDaggerKeyboard::KeyA;
        EDaggerKeyboard spaceKey = EDaggerKeyboard::KeySpace;


        Vector3 input{ 0, 1, 0 };
    };

    enum class Option { Yes, No};

    struct EndScreenState {
        Option selectedOption = Option::Yes;
    };

    class GameStateInputSystem : public System
    { 
    public:
        inline String SystemName() { return "Game State Input System"; }

        void Run() override;
        void SpinUp() override;
        void WindDown() override;
        void OnKeyboardEvent(KeyboardEvent kEvent_);
    };
}

namespace academic_life {

    void GameOverScreen();
    void WinScreen();

}