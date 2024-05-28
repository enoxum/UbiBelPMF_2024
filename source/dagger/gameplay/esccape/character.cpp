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

Character::Character(Entity entity, const std::string& input_, const std::string& spritesheet_, const std::string& animation_, const ColorRGB& color_, const Vector2& position_, int id)
    : entity(entity)
{
    auto& reg = Engine::Registry();

    sprite = &reg.get_or_emplace<Sprite>(entity);
    animator = &reg.get_or_emplace<Animator>(entity);
    input = &reg.get_or_emplace<InputReceiver>(entity);
    character = &reg.get_or_emplace<esccape::EsccapeCharacter>(entity);
    transform = &reg.get_or_emplace<Transform>(entity);
    collision = &reg.get_or_emplace<SimpleCollision>(entity);


    float ratio = sprite->size.x / sprite->size.y;
    float size = 50;

    sprite->size = { size, size * ratio };
    sprite->scale = { 3, 3 };
    sprite->position = { position_, 0.0f };
    sprite->color = { color_, 1.0f };

    transform->position = { position_, 0.0f };
    collision->size.x = sprite->size.x;
    collision->size.y = sprite->size.y;
    
    character->speed = 100.f;
    character->id = id;
    character->healthSystem.SetCurrentHealth(10);
    character->healthSystem.SetMaxHealth(10);
    character->healthSystem.SetCharacterID(id);
    

    ATTACH_TO_FSM(CharacterControllerFSM, entity);

    AssignSprite(*sprite, spritesheet_);
    AnimatorPlay(*animator, animation_);

    if (!input_.empty())
    {
        input->contexts.push_back(input_);
    }
}


Character::~Character() {
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
