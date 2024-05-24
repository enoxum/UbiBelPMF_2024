#include "character.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"


#include "blackboard_manager.h"
#include "esccape_controller.h"

using namespace esccape;


Character::Character(Entity entity, Sprite& sprite, Animator& animator, InputReceiver& input, esccape::EsccapeCharacter& character, Transform& transform, SimpleCollision& collision)
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

    return Character(entity, sprite, anim, input, character, transform, collision);
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

    chr.sprite.scale = { 2.5, 2.5 };
    chr.sprite.position = { position_, 0.0f };
    chr.sprite.color = { color_, 1.0f };
    chr.character.id = id;
    chr.transform.position = { position_, 0.0f };
    chr.collision.size.x = chr.sprite.scale.x;
    chr.collision.size.y = chr.sprite.scale.y;

    AssignSprite(chr.sprite, spritesheet_);
    AnimatorPlay(chr.animator, animation_);

    if (!input_.empty())
    {
        chr.input.contexts.push_back(input_);
    }

    chr.character.speed = 50;

    return chr;
}

Entity Character::getEntity() const
{
    return entity;
}

Sprite& Character::getSprite()
{
    return sprite;
}

Animator& Character::getAnimator()
{
    return animator;
}

InputReceiver& Character::getInputReceiver()
{
    return input;
}

esccape::EsccapeCharacter& Character::getEsccapeCharacter()
{
    return character;
}


void esccape::Character::CheckCollisions()
{
    BlackboardManager& bbManager = BlackboardManager::GetInstance();
    auto& fsmState = Engine::Registry().get<CharacterControllerFSM::StateComponent>(entity);
    CheckCollisionsFSM(fsmState, bbManager);
}

void esccape::Character::Run()
{
    CheckCollisions();
    if (!BlackboardManager::GetInstance().HasCollided())
        printf("nistaaa");
    else
        printf("yaaaaaaaaaaaaaaaay");

    if (BlackboardManager::GetInstance().HasCollided())
    {
        auto collidedWith = BlackboardManager::GetInstance().CollidedWith();
        ResolveCollision(Engine::Registry().get<CharacterControllerFSM::StateComponent>(entity), collidedWith, BlackboardManager::GetInstance());
    }
}



 //////////////////////*********************************/////////////////////////

//#include "player.h"
//
//#include "core/engine.h"
//#include "core/game/transforms.h"
//#include "core/graphics/sprite.h"
//
//#include "gameplay/common/simple_collisions.h"
//
//#include <algorithm>
//#include <execution>
//#include <chrono> // For std::chrono
//#include <thread> // For std::this_thread
//
//using namespace dagger;
//using namespace esccape;
//
//esccape::Player::Player()
//{
//}
//
//esccape::Player::Player(PlayerEntity playerEntity, std::function<void(int)> healthChangedCallback2) :
//    player(playerEntity), healthChangedCallback(healthChangedCallback2)
//{
//}
//
//void Player::SpinUp()
//{
//    Engine::Dispatcher().sink<KeyboardEvent>().connect<&Player::OnKeyboardEvent>(this);
//}
//
//void Player::WindDown()
//{
//    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&Player::OnKeyboardEvent>(this);
//}
//
//void Player::OnKeyboardEvent(KeyboardEvent kEvent_)
//{
//    Engine::Registry().view<ControllerMapping>().each([&](ControllerMapping& ctrl_)
//        {
//
//            if (kEvent_.key == ctrl_.leftKey && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
//            {
//                ctrl_.input.x = -1;
//            }
//            else if (kEvent_.key == ctrl_.leftKey && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x < 0)
//            {
//                ctrl_.input.x = 0;
//            }
//            else if (kEvent_.key == ctrl_.rightKey && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
//            {
//                ctrl_.input.x = 1;
//            }
//            else if (kEvent_.key == ctrl_.rightKey && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x > 0)
//            {
//                ctrl_.input.x = 0;
//            }
//            else if (kEvent_.key == ctrl_.downKey && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
//            {
//                ctrl_.input.y = -1;
//            }
//            else if (kEvent_.key == ctrl_.downKey && kEvent_.action == EDaggerInputState::Released && ctrl_.input.y < 0)
//            {
//                ctrl_.input.y = 0;
//            }
//            else if (kEvent_.key == ctrl_.upKey && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
//            {
//                ctrl_.input.y = 1;
//            }
//            else if (kEvent_.key == ctrl_.upKey && kEvent_.action == EDaggerInputState::Released && ctrl_.input.y > 0)
//            {
//                ctrl_.input.y = 0;
//            }
//        });
//}
//
//void esccape::Player::setHealthChangedCallback(std::function<void(int)> callback)
//{
//    healthChangedCallback = callback;
//}
//
//
//void Player::Run()
//{
//    //RacingGameFieldSettings fieldSettings;
//    //if (auto* ptr = Engine::GetDefaultResource<RacingGameFieldSettings>())
//    //{
//    //    fieldSettings = *ptr;
//    //}
//
    //auto viewCollisions = Engine::Registry().view<Transform, SimpleCollision>();
    //auto view = Engine::Registry().view<Transform, ControllerMapping, Player>();
    //auto view2 = Engine::Registry().view<Player, Transform, SimpleCollision>();


    //for (auto entity : view)
    //{
    //    auto& t = view.get<Transform>(entity);
    //    auto& ctrl = view.get<ControllerMapping>(entity);
    //    auto& player = view.get<Player>(entity);

    //    auto& col = view2.get<SimpleCollision>(entity);

    //    t.position.x += ctrl.input.x * player.player.speed * Engine::DeltaTime();
    //    t.position.y += ctrl.input.y * player.player.speed * Engine::DeltaTime();

    //    Logger::trace(t.position.x);

    //    // How to have collisions be the same as sprite
    //    if (col.colided)
    //    {
    //        if (Engine::Registry().valid(col.colidedWith))
    //        {
    //            SimpleCollision& collision = viewCollisions.get<SimpleCollision>(col.colidedWith);
    //            Transform& transform = viewCollisions.get<Transform>(col.colidedWith);

    //            Vector2 collisionSides = col.GetCollisionSides(t.position, collision, transform.position);

    //            while(col.IsCollided(t.position, collision, transform.position))
    //            {                   
    //                if (std::abs(collisionSides.x) > 0 && col.IsCollided(t.position, collision, transform.position))
    //                {
    //                    t.position.x -= t.position.x < 0 ? -1 : 1;
    //                }
    //                if (std::abs(collisionSides.y) > 0 && col.IsCollided(t.position, collision, transform.position))
    //                {
    //                    t.position.y -= t.position.y < 0 ? -1 : 1;
    //                }
    //            }
    //        }
    //    }
//    }
//}
//
//float esccape::Player::getSpeed()
//{
//    return player.speed;
//}
//
//void esccape::Player::setSpeed(float newSpeed)
//{
//    player.speed = newSpeed;
//}
//
//int esccape::Player::getHealth()
//{
//    return player.health;
//}
//
//void esccape::Player::setHealth(int newHealth)
//{
//    player.health = newHealth;
//}
//
