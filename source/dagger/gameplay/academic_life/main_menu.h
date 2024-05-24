#pragma once

#include "core/graphics/text.h"
#include "core/system.h"
#include "core/core.h"
#include "gameplay/common/particles.h"


using namespace dagger;

namespace academic_life
{
    struct MenuControllerMapping
    {
        EDaggerKeyboard spaceKey = EDaggerKeyboard::KeySpace;
        EDaggerKeyboard muteKey = EDaggerKeyboard::KeyM;
        EDaggerKeyboard quitKey = EDaggerKeyboard::KeyQ;

        Vector3 input{ 0, 1, 0 };
    };

    struct MainMenu
    {
        String logoTxt = "Academic Life";
        Vector2 logoPos{ 0,0 };
        String playGameTxt = "Press Space to start";
        Vector2 playGamePos{ 0,0 };

        int play = 0;
        int mute = 1;
        int quit = 0;

    };


    class MainMenuInputSystem : public System
    {
    public:
        inline String SystemName() { return "Main Menu Input System"; }

        void Run() override;
        void SpinUp() override;
        void WindDown() override;

    private:
        void OnKeyboardEvent(KeyboardEvent kEvent_);
    };
}


