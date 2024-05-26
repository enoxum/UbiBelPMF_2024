#pragma once

#include "core/graphics/text.h"
#include "core/system.h"
#include "core/core.h"
#include "core/engine.h"
#include "gameplay/common/particles.h"
#include "core/graphics/sprite.h"


using namespace dagger;

namespace academic_life
{
    struct MenuControllerMapping
    {
        EDaggerKeyboard spaceKey = EDaggerKeyboard::KeySpace;
        EDaggerKeyboard muteKey = EDaggerKeyboard::KeyM;
       


        Vector2 input{ 0, 1 };
    };

  

    struct MainMenu
    {
        String logoTxt = "Academic Life";
        Vector2 logoPos{ 0,0 };
        String playGameTxt = "Press Space to start";
        Vector2 playGamePos{ 0,0 };



        int play = 0;
        int mute = 1;



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


