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

    struct MuteButton {

        Entity entity;
        Sprite& sprite;

        static MuteButton Get(Entity entity)
        {
            auto& reg = Engine::Registry();
            auto& sprite = reg.get_or_emplace<Sprite>(entity);

            return MuteButton{ entity, sprite };
        }

        static MuteButton Create(
            Vector2 position_ = { 0, 0 }, String spritePath = "AcademicLife:student")
        {
            auto& reg = Engine::Registry();
            auto entity = reg.create();


            auto btn = MuteButton::Get(entity);

            btn.sprite.scale = { 1, 1 };
            btn.sprite.position = { position_, 0.6f };;

            AssignSprite(btn.sprite, spritePath);

            return btn;
        }

        static void ChangeSprite(String spritePath = "AcademicLife:cigarette")
        {
            auto& reg = Engine::Registry();
            auto entity = reg.create();
            auto btn = MuteButton::Get(entity);

        }


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

        MuteButton *muteBtn;

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


