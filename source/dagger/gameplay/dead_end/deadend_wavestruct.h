#pragma once

#include "deadend_player.h"
#include "deadend_health.h"

using namespace dagger;
using namespace dead_end;

namespace dead_end {

    struct Wave {
        int waveNumber{ 1 };
        int waveSize{ 5 };
        std::string weapon{ "Pistol" };
        int healthResetPercentage{ 0 };
        bool bossWave{ false };
    };

    class WaveSystem
        : public System{

        inline String SystemName() override { return "Wave system"; }

        void Run() override;

        public:
            void UpdateWave(Wave& wave_, Player& player_, Health& health_);
            void SetWave(Wave& wave_);

            void ApplyWaveEffects(Wave& wave_, Player& player_, Health& health_);
    };


} 