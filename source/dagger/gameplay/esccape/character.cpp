#include "character.h"
#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include "blackboard_manager.h"
#include "esccape_controller.h"

using namespace esccape;

Character::Character(Entity entity, Sprite* sprite, Animator* animator, InputReceiver* input, esccape::EsccapeCharacter* character, Transform* transform, SimpleCollision* collision)
    : entity(entity), sprite(sprite), animator(animator), input(input), character(character), transform(transform), collision(collision)
{
}

Character Character::Get(Entity entity)
{
    auto& reg = Engine::Registry();
    auto& sprite = reg.get_or_emplace<Sprite>(entity);
    auto& anim = reg.get_or_emplace<Animator>(entity);
    auto& input = reg.get_or_emplace<InputReceiver>(entity);
    auto& character = reg.get_or_emplace<esccape::EsccapeCharacter>(entity);
    auto& transform = reg.get_or_emplace<Transform>(entity);
    auto& collision = reg.get_or_emplace<SimpleCollision>(entity);

    return Character(entity, &sprite, &anim, &input, &character, &transform, &collision);
}

Character Character::Create(
    const std::string& input_,
    const std::string& spritesheet_,
    const std::string& animation_,
    const ColorRGB& color_,
    const Vector2& position_,
    int id
)
{
    auto& reg = Engine::Registry();
    auto entity = reg.create();

    ATTACH_TO_FSM(CharacterControllerFSM, entity);

    auto chr = Character::Get(entity);

    float ratio = chr.sprite->size.y / chr.sprite->size.y;
    float size = 50;

    chr.sprite->size = { size, size * ratio };

    chr.sprite->scale = { 3, 3 };
    chr.sprite->position = { position_, 0.0f };
    chr.sprite->color = { color_, 1.0f };
    chr.character->id = id;
    chr.transform->position = { position_, 0.0f };
    chr.collision->size.x = chr.sprite->size.x;
    chr.collision->size.y = chr.sprite->size.y;

    AssignSprite(*chr.sprite, spritesheet_);
    AnimatorPlay(*chr.animator, animation_);

    if (!input_.empty())
    {
        chr.input->contexts.push_back(input_);
    }

    chr.character->speed = 80;

    return chr;
}

Entity Character::getEntity() const
{
    return entity;
}

Sprite* Character::getSprite()
{
    return sprite;
}

Animator* Character::getAnimator()
{
    return animator;
}

InputReceiver* Character::getInputReceiver()
{
    return input;
}

esccape::EsccapeCharacter* Character::getEsccapeCharacter()
{
    return character;
}

std::pair<Entity, Entity> Character::CheckCollisions()
{
    BlackboardManager& bbManager = BlackboardManager::GetInstance();
    auto& fsmState = Engine::Registry().get<CharacterControllerFSM::StateComponent>(entity);
    return CheckCollisionsFSM(fsmState, bbManager);
}
