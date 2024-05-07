#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace esccape
{
    class EscPlayer
    {
    public:
        EscPlayer();
        Vector2 getSpeed();

    private:
        Vector2 speed{ 0, 0 };
        int health{ 5 };
        int water{ 5 };

    };
}