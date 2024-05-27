#include "deadend_wavestruct.h"
#include "core/engine.h"
#include "deadend_player.h"
#include "deadend_health.h"
#include "deadend_enemy.h"
#include "deadend_map.h"

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

                if (enemyView.empty()) // or enemyView.empty()
                {
                    //player.killCount++;
                    wave.waveNumber++;
                    SetWave(wave);
                    UpdateWave(wave, player, health);
                }
            }
           
            
            break;

        case true:
            // spawn a boss.
            break;

        default:
            break;
        }
    }
   
}



void WaveSystem::SetWave(Wave& wave) {
    switch (wave.waveNumber) {
    case 1:
        wave.weapon = "Pistol";
        wave.healthResetPercentage = 0;
        break;
    case 2:
        wave.weapon = "Gun";
        wave.healthResetPercentage = 0;
        break;
    case 3:
        wave.weapon = "Shotgun";
        wave.healthResetPercentage = 50;
        break;
    case 4:
        wave.weapon = "Machine gun";
        wave.healthResetPercentage = 100;
        wave.bossWave = true;
        wave.waveNumber = 1;
        break;
    default:
        break;
    }
}

void WaveSystem::UpdateWave(Wave& wave_, Player& player_, Health& health_) {
    ApplyWaveEffects(wave_, player_, health_);
    loadEnemies(4, 55);
    // generate another wave.
}

void WaveSystem::ApplyWaveEffects(Wave& wave_, Player& player_, Health& health_) {
    player_.weaponType = wave_.waveNumber;
    if (wave_.healthResetPercentage > 0) {
        health_.currentHealth = (health_.maxHealth * wave_.healthResetPercentage) / 100;
    }
}
