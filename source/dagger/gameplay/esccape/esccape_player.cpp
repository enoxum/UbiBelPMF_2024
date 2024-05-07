#include "core/system.h"
#include "core/core.h"

#include "esccape_player.h"

using namespace dagger;

namespace esccape
{
    EscPlayer::EscPlayer() {}

    Vector2 EscPlayer::getSpeed() {
        return speed;
    }
}