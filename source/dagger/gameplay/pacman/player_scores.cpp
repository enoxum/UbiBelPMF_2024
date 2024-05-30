#include "player_scores.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "gameplay/common/simple_collisions.h"
#include "gameplay/pacman/pingpong_ball.h"
#include "gameplay/pacman/pacman_main.h"

using namespace pacman;

int PlayerScoresSystem::s_FieldWidth = 20;
int PlayerScoresSystem::s_FieldHeight = 10;
Float32 PlayerScoresSystem::s_TileSize = 1.f;

void PlayerScoresSystem::Run()
{
    auto view = Engine::Registry().view<PingPongBall, Transform>();

    int ballOnField = 0;

    for (auto entity : view)
    {
        auto &ball = view.get<PingPongBall>(entity);
        auto &t = view.get<Transform>(entity);

        if (ball.reachedGoal && !ball.processed)
        {
            ball.speed = { 0,0,0 };

            if (ball.playerOneScored)
            {
                m_goalsPlayerOne++;
                t.position = { (-(s_FieldWidth+3) / 2.f) * s_TileSize,  s_TileSize * m_goalsPlayerOne, 0 };
            }
            else
            {
                m_goalsPlayerTwo++;
                t.position = { ((s_FieldWidth+3) / 2.f) * s_TileSize, s_TileSize * m_goalsPlayerTwo, 0 };
            }

            ball.processed = true;
        }

        if (!ball.reachedGoal)
        {
            ballOnField++;
        }
    }

    //if (ballOnField == 0)
    //{
    //    // TODO: set speed to be random in both directions. Done without if for better performnce 
    //    CreatePingPongBall(s_TileSize, ColorRGBA(1, 1, 1, 1), { (rand() % 10 + 4)*((rand() % 2 + 2)%3-1),(rand() % 10 + 4) * ((rand() % 2 + 2) % 3 - 1),0 },   { 0,rand()%(s_FieldHeight / 2),0 });
    //}
}
