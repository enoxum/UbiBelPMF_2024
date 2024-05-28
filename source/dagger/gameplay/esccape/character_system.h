#include "core/system.h"
#include "character.h"

using namespace dagger;

namespace esccape
{
    class CharacterSystem
        : public System
    {
    public:
        bool m_Restart = false;
        int m_winnerID;

        String SystemName() override {
            return "Character System";
        }
        void SpinUp();
        void WindDown();

        void Run() override;

    private:
        void OnEndOfFrame();
    };
}
