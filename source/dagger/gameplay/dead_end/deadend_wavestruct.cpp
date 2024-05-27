#include "deadend_wavestruct.h"
#include "core/engine.h"
#include "deadend_player.h"
#include "deadend_health.h"
#include "deadend_enemy.h"
#include "deadend_map.h"
#include <gameplay/common/simple_collisions.h>
#include <core/game/transforms.h>
#include "core/engine.h"
#include "deadend_crosshair.h"
#include "deadend_camera.h"

using namespace dead_end;

void dead_end::WaveSystem::Run()
{
    auto view = Engine::Registry().view<Wave>();
    auto playerView = Engine::Registry().view<Player, Health>();
    auto enemyView = Engine::Registry().view<DeadEndEnemy>();
    for(auto entity : view)
    {
        auto& wave = view.get<Wave>(entity);
        
        switch (wave.bossWave)
        {
        case false:
            
            for (auto entityP : playerView)
            {
                auto& player = playerView.get<Player>(entityP);
                auto& health = playerView.get<Health>(entityP);

                if (enemyView.empty()) 
                {
                    //player.killCount++;
                    wave.waveNumber++;
                    SetWave(wave);
                    UpdateWave(wave, player, health);
                }
            }
           
            
            break;

        case true:

            auto bossView = Engine::Registry().view<DeadEndEnemy>();

            if (bossView.empty())
            {
                gameOver();
            }

            break;

        
        }
    }
   
}



void WaveSystem::SetWave(Wave& wave) {
    switch (wave.waveNumber) {
    case 1:
        wave.weapon = 1;
        wave.healthResetPercentage = 0;
        break;
    case 2:
        wave.weapon = 2;
        wave.healthResetPercentage = 0;
        break;
    case 3:
        wave.weapon = 3;
        wave.healthResetPercentage = 50;
        break;
    case 4:
        wave.weapon = 3;
        wave.healthResetPercentage = 100;
        wave.bossWave = true;
        wave.waveSize = 1;
        break;
    default:
        wave.waveSize = 0;
        exit(0);
        break;
    }
}

void WaveSystem::UpdateWave(Wave& wave_, Player& player_, Health& health_) {
    ApplyWaveEffects(wave_, player_, health_);
    if (!wave_.bossWave)
        loadEnemies(wave_.zPos, wave_.playerSize);
    else
        loadBoss(wave_.zPos);
        // generate another wave.
}

void WaveSystem::ApplyWaveEffects(Wave& wave_, Player& player_, Health& health_) {
    player_.weaponType = wave_.weapon;
    if (wave_.healthResetPercentage > 0) {
        health_.currentHealth += (health_.maxHealth * wave_.healthResetPercentage) / 100;
    }
}

void dead_end::WaveSystem::loadBoss(Float32 zPos)
{
    // spawn a boss.

    auto& engine = Engine::Instance();
    auto& reg = engine.Registry();
    auto entity = reg.create();
    auto& enemy = reg.emplace<DeadEndEnemy>(entity);
    auto& col = reg.emplace<SimpleCollision>(entity);
    auto& sprite = reg.emplace<Sprite>(entity);
    auto& transform = reg.emplace<Transform>(entity);
    auto& health = reg.emplace<Health>(entity);

    enemy.speed = 50.f;
    enemy.meleeDamage = 15.f;
    enemy.is_boss = true;

    health.currentHealth = 200.f;
    health.maxHealth = 200.f;

    AssignSprite(sprite, "dead_end:Enemy:zombie_3");
    sprite.size.x = 150.f;
    sprite.size.y = 150.f;

    col.size.x = 20.f;
    col.size.y = 20.f;

    transform.position.x = 400.f;
    transform.position.y = 400.f;
    transform.position.z = zPos;
}

void dead_end::gameOver()
{

    auto playerView = Engine::Registry().view<Transform, Player>();
    auto crosshairView = Engine::Registry().view<Aim>();
    auto enemyView = Engine::Registry().view<DeadEndEnemy>();
    

    auto playerEntity = playerView.front();
    auto& playerPos = playerView.get<Transform>(playerEntity);

    auto crosshairEntity = crosshairView.front();

    auto& engine = Engine::Instance();
    auto& reg = engine.Registry();
    auto entity = reg.create();

    auto& sprite = reg.emplace<Sprite>(entity);
    auto& transform = reg.emplace<Transform>(entity);

    AssignSprite(sprite, "dead_end:GameOver:game_over_2");
    sprite.UseAsUI();
    sprite.size.x = 800;
    sprite.size.y = 600;

    transform.position.z = 0.f;
    transform.position.x = playerPos.position.x;
    transform.position.y = playerPos.position.y;

}
