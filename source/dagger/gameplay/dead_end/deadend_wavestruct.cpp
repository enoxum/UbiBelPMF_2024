#include "deadend_wavestruct.h"
#include "core/engine.h"

using namespace dead_end;

void WaveSystem::Initialize() {
    currentWave.waveNumber = 1;
    currentWave.weapon = "Pistol";
    currentWave.healthResetPercentage = 0;
}

void WaveSystem::SetWave(int waveNumber) {
    currentWave.waveNumber = waveNumber;
    switch (waveNumber) {
    case 1:
        currentWave.weapon = "Pistol";
        currentWave.healthResetPercentage = 0;
        break;
    case 2:
        currentWave.weapon = "Gun";
        currentWave.healthResetPercentage = 0;
        break;
    case 3:
        currentWave.weapon = "Strong Rifle";
        currentWave.healthResetPercentage = 50;
        break;
    case 4:
        currentWave.weapon = "Machine gun";
        currentWave.healthResetPercentage = 100;
        break;
    default:
        break;
    }
}

void WaveSystem::UpdateWave(Player& player, Health& health) {
    ApplyWaveEffects(player, health);
}

void WaveSystem::ApplyWaveEffects(Player& player, Health& health) {
    player.weaponType = currentWave.waveNumber;
    if (currentWave.healthResetPercentage > 0) {
        health.currentHealth = (health.maxHealth * currentWave.healthResetPercentage) / 100;
    }
}
