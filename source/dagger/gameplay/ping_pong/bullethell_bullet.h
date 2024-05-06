#pragma once

#include "core/system.h"
#include "core/core.h"
#include <queue>

using namespace dagger;

namespace ping_pong{

    struct Damage{
        float damageAmount{1.f};

    };

    struct Bullet : public Damage{
        Entity owner;
        Vector3 velocity{0,0, 0};
    };

    struct Damageable{
        bool isDamageable{true};
        float lastDamaged{0.f};
        float hurtDuration{0.1f};
    };

    struct DamageEvent{
        float damageAmount{0};
        Entity owner;
        Entity target;
    };

    class DamageSystem : public System{
    public:

        inline String SystemName() { return "Damage System"; }


        void Run() override;
        void SpinUp() override;
        void WindDown() override;
        void OnDamageEvent(DamageEvent dEvent);
    };
    class BulletSystem : public System{
    public:

        inline String SystemName() { return "Bullet System"; }

        void Run() override;
    };


}