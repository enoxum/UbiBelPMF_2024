#pragma once
\

#include "deadend_player.h"
#include "deadend_health.h"

using namespace dagger;
using namespace dead_end;

namespace dead_end {

    struct Wave {
        int waveNumber;
        std::string weapon;
        int healthResetPercentage;
    };

    class WaveSystem {
    public:
        void Initialize();
        void UpdateWave(Player& player, Health& health);
        void SetWave(int waveNumber);

    private:
        Wave currentWave;
        void ApplyWaveEffects(Player& player, Health& health);
    };

} 