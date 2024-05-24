#include "core/system.h"
#include "character.h"

using namespace dagger;

namespace esccape
{
    class CharacterSystem
        : public System
    {
    public:
        String SystemName() override {
            return "Character System";
        }
        void Run() override;
    };
}
