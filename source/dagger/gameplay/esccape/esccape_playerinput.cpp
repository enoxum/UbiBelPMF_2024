#include "esccape_playerinput.h"

#include "core/engine.h"
#include "core/game/transforms.h"

using namespace dagger;
using namespace esccape;

Float32 EsccapePlayerInputSystem::s_BoarderRadius = 10;
Float32 EsccapePlayerInputSystem::s_PlayerSpeed = 1.f;

void EsccapePlayerInputSystem::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&EsccapePlayerInputSystem::OnKeyboardEvent>(this);
}

void EsccapePlayerInputSystem::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&EsccapePlayerInputSystem::OnKeyboardEvent>(this);
}

void EsccapePlayerInputSystem::OnKeyboardEvent(KeyboardEvent kEvent_)
{
    Engine::Registry().view<ControllerMapping>().each([&](ControllerMapping& ctrl_)
        {
           // todo
        });
}

void EsccapePlayerInputSystem::Run()
{
    // todo
   
}
