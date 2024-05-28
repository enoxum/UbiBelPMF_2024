
#include "core/system.h"
#include "core/core.h"
#include <string>
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/game/transforms.h"
#include "gameplay/common/simple_collisions.h"
#include "esccape_controller.h"
#include "character_controller.h"

using namespace dagger;

namespace esccape
{
    class Character 
    {
    public:
        Character(Entity entity, Sprite* sprite, Animator* animator, InputReceiver* input, esccape::EsccapeCharacter* character, Transform* transform, SimpleCollision* collision);
        Character(Entity entity, const std::string& input_, const std::string& spritesheet_, const std::string& animation_, const ColorRGB& color_, const Vector2& position_, int id);
        ~Character();

        Entity getEntity() const;
        Sprite* getSprite();
        Animator* getAnimator();
        InputReceiver* getInputReceiver();
        esccape::EsccapeCharacter* getEsccapeCharacter();


        std::pair<Entity, Entity> CheckCollisions();
        esccape::EsccapeCharacter* character;

    private:
        Entity entity = entt::null;
        Sprite* sprite;
        Animator* animator;
        InputReceiver* input;
        SimpleCollision* collision;
        Transform* transform;
    };
}
