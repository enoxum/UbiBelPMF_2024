#include "pingpong_tools.h"

#if defined(DAGGER_DEBUG)

#include "core/engine.h"

#include "gameplay/pacman/ping_pong_main.h"
#include "gameplay/pacman/player_scores.h"

using namespace dagger;
using namespace pacman;

void pacman::PingPongTools::SpinUp()
{
    Engine::Dispatcher().sink<NextFrame>().connect<&PingPongTools::OnEndOfFrame>(this);

    Engine::Dispatcher().sink<ToolMenuRender>().connect<&PingPongTools::RenderToolMenu>(this);
}

void pacman::PingPongTools::WindDown()
{
    Engine::Dispatcher().sink<NextFrame>().disconnect<&PingPongTools::OnEndOfFrame>(this);

    Engine::Dispatcher().sink<ToolMenuRender>().disconnect<&PingPongTools::RenderToolMenu>(this);
}

void PingPongTools::RenderToolMenu()
{
    if (ImGui::BeginMenu("Ping Pong"))
    {
        if (ImGui::MenuItem("Restart"))
        {
            dagger::Engine::ToggleSystemsPause(false);
            m_RestartGame = true;
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Generate ball"))
        {
            m_GenerateBall = true;
        }

        ImGui::EndMenu();
    }
}

void pacman::PingPongTools::Run()
{
}

void PingPongTools::OnEndOfFrame()
{
    if (m_RestartGame)
    {
        m_RestartGame = false;
        Engine::Registry().clear();

        pacman::SetupWorld();
    }

    /*if (m_GenerateBall)
    {
        m_GenerateBall = false;

        pacman::CreatePingPongBall(PlayerScoresSystem::GetTileSize(), ColorRGBA(1, 1, 1, 1),
            { (1 - 2 * (rand() % 2)) * (rand()%10 + 4),rand()%10 + 4,0 },   { 0,0,0 });
    }*/
}

#endif //defined(DAGGER_DEBUG)