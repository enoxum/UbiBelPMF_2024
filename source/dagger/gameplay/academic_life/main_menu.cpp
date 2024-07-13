#include "main_menu.h"

#include "core/engine.h"
#include "core/game/transforms.h"


#include "gameplay/academic_life/academic_life_logic.h"
#include "gameplay/academic_life/academic_life_main.h"
#include <core/audio.h>

using namespace dagger;
using namespace academic_life;

void MainMenuInputSystem::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect < &MainMenuInputSystem::OnKeyboardEvent >(this);

}

void MainMenuInputSystem::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&MainMenuInputSystem::OnKeyboardEvent>(this);
}

void MainMenuInputSystem::OnKeyboardEvent(KeyboardEvent kEvent_)
{
    Engine::Registry().view<MenuControllerMapping>().each([&](MenuControllerMapping& ctrl_)
        {
            if (kEvent_.key == ctrl_.spaceKey && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
            {
                ctrl_.input.x = 1;
            }
            else if (kEvent_.key == ctrl_.muteKey && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
            {
                if (ctrl_.input.y == 1)
                {
                    Engine::GetDefaultResource<Audio>()->Stop();
                
                }
                else
                    Engine::GetDefaultResource<Audio>()->PlayLoop("music");
                ctrl_.input.y *= -1;
            }


        }
    );
}


void MainMenuInputSystem::Run()
{


    auto view = Engine::Registry().view<Transform, MenuControllerMapping, MainMenu>();
    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& ctrl = view.get<MenuControllerMapping>(entity);
        auto& mainMenu = view.get<MainMenu>(entity);
   
        mainMenu.play = ctrl.input.x;
        mainMenu.mute = ctrl.input.y;
        if (mainMenu.play == 1)
        {
            Engine::Registry().clear();
            academic_life::SetupWorld();
        }
     

     
     
    }


}

namespace academic_life
{

}