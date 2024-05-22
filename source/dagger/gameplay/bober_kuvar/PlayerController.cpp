#include "PlayerController.h"
#include "Bullet.h"
#include "Ranged.h"
#include "Melee.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include <core/graphics/window.h>
#include <core/graphics/sprite.h>

#include <cmath>
#include <math.h>

using namespace bober_game;

double PlayerController::playerSpeed = 100.0;
Vector2 playerPos{ 0,0 };
//std::vector<Bullet> bullets;

void PlayerController::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&PlayerController::OnKeyboardEvent>(this);
    Engine::Dispatcher().sink<CursorEvent>().connect<&PlayerController::OnCursorMoveEvent>(this);
    Engine::Dispatcher().sink<MouseEvent>().connect<&PlayerController::OnMouseEvent>(this);
}

void PlayerController::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&PlayerController::OnKeyboardEvent>(this);
    Engine::Dispatcher().sink<CursorEvent>().disconnect<&PlayerController::OnCursorMoveEvent>(this);
    Engine::Dispatcher().sink<MouseEvent>().disconnect<&PlayerController::OnMouseEvent>(this);
}

void PlayerController::OnKeyboardEvent(KeyboardEvent kEvent_)
{
    Engine::Registry().view<ControllerMapping>().each([&](ControllerMapping& ctrl_)
    {
        if (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held)
        {
            if (kEvent_.key == ctrl_.upKey)
                ctrl_.input.y = 1;
            if (kEvent_.key == ctrl_.downKey)
                ctrl_.input.y = -1;

            if (kEvent_.key == ctrl_.leftKey)
                ctrl_.input.x = -1;
            if (kEvent_.key == ctrl_.rightKey)
                ctrl_.input.x = 1;

            if (kEvent_.key == ctrl_.num1) {
                ctrl_.index = 0;
            }
            if (kEvent_.key == ctrl_.num2) {
                ctrl_.index = 1;
            }
        }
        else if (kEvent_.action == EDaggerInputState::Released)
        {
            if(kEvent_.key == ctrl_.upKey || kEvent_.key == ctrl_.downKey)
                ctrl_.input.y = 0;

            if (kEvent_.key == ctrl_.leftKey || kEvent_.key == ctrl_.rightKey)
                ctrl_.input.x = 0;
        }
    });
}

void PlayerController::OnCursorMoveEvent(CursorEvent cursor_)
{
    auto viewCursor = Engine::Registry().view<Cursor>();
    for (auto entity : viewCursor)
    {
        auto& c = viewCursor.get<Cursor>(entity);
        auto* camera = Engine::GetDefaultResource<Camera>();

        c.position.x = cursor_.x - camera->size.x / 2;
        c.position.y = -cursor_.y + camera->size.y / 2;
    }
}

void PlayerController::OnMouseEvent(MouseEvent input_)
{
    bool isMousePressed = input_.action == EDaggerInputState::Pressed;

    auto viewCursor = Engine::Registry().view<Cursor>();
    for (auto entity : viewCursor)
    {
        auto& c = viewCursor.get<Cursor>(entity);
        c.isMouseBtnPressed = isMousePressed;
    }
    auto viewRanged = Engine::Registry().view<RangedWeaponSystem,ShootEvent>();//<RangedWeaponSystem,ShootEvent>
    for (auto entity : viewRanged)
    {
        auto& r = viewRanged.get<RangedWeaponSystem>(entity);
        auto& s = viewRanged.get<ShootEvent>(entity);
        r.isMouseBtnPressed = isMousePressed;
        //Event za kreiranje bullet-a
        if (r.isMouseBtnPressed && r.isActive) {
            s.speed = 150.f;
            s.position = r.position;
            Engine::Dispatcher().trigger<ShootEvent>(s);//prosledjujemo s
        }
    }
    auto viewMelee = Engine::Registry().view<MeleeWeaponSystem,SlashEvent>();// <MeleeWeaponSystem, SlashEvent>
    for (auto entity : viewMelee)
    {
        auto& m = viewMelee.get<MeleeWeaponSystem>(entity);
        auto& s = viewMelee.get<SlashEvent>(entity);
        m.isMouseBtnPressed = isMousePressed;
        if (m.isMouseBtnPressed && m.isActive) {

            Engine::Dispatcher().trigger<SlashEvent>(s);
        }
    }
}

void PlayerController::Run()
{
    bool focusRanged=true;
    double cos_, sin_;
    Vector2 dir{ 0,0 };
    auto view = Engine::Registry().view<Transform, ControllerMapping>();
    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& ctrl = view.get<ControllerMapping>(entity);
        if (ctrl.index) {
            focusRanged = true;
        }
        else {
            focusRanged = false;
        }

        if (ctrl.input == Vector2{0.0f, 0.0f})
            break;
        double normalized = 1 / sqrt(ctrl.input.x * ctrl.input.x + ctrl.input.y * ctrl.input.y);

        t.position.x += normalized * ctrl.input.x * playerSpeed * Engine::DeltaTime();
        t.position.y += normalized * ctrl.input.y * playerSpeed * Engine::DeltaTime();
        playerPos.x = t.position.x;
        playerPos.y = t.position.y;
    }
    auto viewCursor = Engine::Registry().view<Transform, Sprite, Cursor>();
    for (auto entity : viewCursor)
    {
        auto& t = viewCursor.get<Transform>(entity);
        auto& s = viewCursor.get<Sprite>(entity);
        auto& c = viewCursor.get<Cursor>(entity);
        double theta = atan2(c.position.y , c.position.x);
        cos_ = cos(theta);
        sin_ = sin(theta);
        t.position.x = playerPos.x + 50 * cos_;
        t.position.y = playerPos.y + 50 * sin_;
        s.color.r = c.isMouseBtnPressed ? 0 : 1;
    }
    auto viewRanged = Engine::Registry().view<Transform, Sprite, RangedWeaponSystem>();
    for (auto entity : viewRanged)
    {
        auto& t = viewRanged.get<Transform>(entity);
        auto& s = viewRanged.get<Sprite>(entity);
        auto& r = viewRanged.get<RangedWeaponSystem>(entity);
        r.isActive = focusRanged;
        if (r.isActive) {
            s.color.b = 0;
            //Missing: Activate sprite transparency or add sprite back
            r.position.x=t.position.x = playerPos.x + 8 * cos_;
            r.position.y=t.position.y = playerPos.y + 8 * sin_;
            dir.x = t.position.x - playerPos.x;
            dir.y = t.position.y - playerPos.y;
            s.color.r = r.isMouseBtnPressed ? 0 : 1;
        }
        else {
            s.color.b = 1;
            //Missing: Make sprite transparent or remove sprite.
        }
    }
    auto viewMelee = Engine::Registry().view<Transform, Sprite, MeleeWeaponSystem>();
    for (auto entity : viewMelee)
    {
        auto& t = viewMelee.get<Transform>(entity);
        auto& s = viewMelee.get<Sprite>(entity);
        auto& m = viewMelee.get<MeleeWeaponSystem>(entity);
        m.isActive = !focusRanged;
        if (m.isActive) {
            s.color.b = 0;
            //Missing: Activate sprite transparency or add sprite back
            t.position.x = playerPos.x + 8 * cos_;
            t.position.y = playerPos.y + 8 * sin_;
            s.color.r = m.isMouseBtnPressed ? 0 : 1;
        }
        else {
            s.color.b = 1;
            //Missing: Make sprite transparent or remove sprite.
        }
    }
    auto viewBullet = Engine::Registry().view<Transform, Sprite, BulletSystem>();
    for (auto entity : viewBullet)
    {
        auto& t = viewBullet.get<Transform>(entity);
        auto& s = viewBullet.get<Sprite>(entity);
        auto& b = viewBullet.get<BulletSystem>(entity);
                            //get<SimpleCollision>
        if (b.firstTime) {
            b.dir = dir;
            float length = sqrt(b.dir.x * b.dir.x + b.dir.y * b.dir.y);

            if (length != 0)
            {
                b.dir.x /= length;
                b.dir.y /= length;
            }
            b.firstTime = false;
        }
        t.position.x += b.dir.x * b.speed * Engine::DeltaTime();
        t.position.y += b.dir.y * b.speed * Engine::DeltaTime();
    }
}
