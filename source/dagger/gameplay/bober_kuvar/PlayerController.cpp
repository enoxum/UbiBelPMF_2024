#include "PlayerController.h"
#include "Bullet.h"
#include "Ranged.h"
#include "Melee.h"
#include "Player.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include <core/graphics/window.h>
#include <core/graphics/sprite.h>
#include <gameplay/common/simple_collisions.h>

#include <cmath>
#include <math.h>

using namespace bober_game;

double PlayerController::playerSpeed = 100.0;
Vector2 playerPos{ 0,0 };
std::unordered_map<int,Bullet*> PlayerController::bullets;

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
        if(kEvent_.key == ctrl_.num1 && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
              ctrl_.index = 0;
      
        if (kEvent_.key == ctrl_.num2 && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held)) 
              ctrl_.index = 1;
      
        if (kEvent_.key == ctrl_.reloadKey && kEvent_.action == EDaggerInputState::Released)
              Engine::Dispatcher().trigger<ReloadEvent>();
      
        //Y
        if (kEvent_.key == ctrl_.upKey && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
        {
            ctrl_.input.y = 1;
        }
        else if (kEvent_.key == ctrl_.upKey && kEvent_.action == EDaggerInputState::Released && ctrl_.input.y > 0)
        {
            ctrl_.input.y = 0;
        }
        else if (kEvent_.key == ctrl_.downKey && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
        {
            ctrl_.input.y = -1;
        }
        else if (kEvent_.key == ctrl_.downKey && kEvent_.action == EDaggerInputState::Released && ctrl_.input.y < 0)
        {
            ctrl_.input.y = 0;
        }
      
        //X
        if (kEvent_.key == ctrl_.rightKey && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
        {
            ctrl_.input.x = 1;
        }
        else if (kEvent_.key == ctrl_.rightKey && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x > 0)
        {
            ctrl_.input.x = 0;
        }
        else if (kEvent_.key == ctrl_.leftKey && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
        {
            ctrl_.input.x = -1;
        }
        else if (kEvent_.key == ctrl_.leftKey && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x < 0)
        {
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
    Vector3 playerPosition;
    bool colidedWithEnemy = false;
  
    auto otherViews = Engine::Registry().view<Transform, SimpleCollision>();
    auto view = Engine::Registry().view<Transform, ControllerMapping, SimpleCollision, MovementData>();
    auto enemyView = Engine::Registry().view<Transform, EnemyData, MovementData, Patrol, SimpleCollision>();
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
        auto& mov = view.get<MovementData>(entity);
        auto& col = view.get<SimpleCollision>(entity);

        playerPosition = t.position;

        if (ctrl.input == Vector2{0.0f, 0.0f})
            break;

        double normalized = 1 / sqrt(ctrl.input.x * ctrl.input.x + ctrl.input.y * ctrl.input.y);

        double move_x = normalized * ctrl.input.x * mov.speed * Engine::DeltaTime();
        double move_y = normalized * ctrl.input.y * mov.speed * Engine::DeltaTime();

        t.position.x += move_x;
        t.position.y += move_y;
      
        playerPosition = t.position;

        if (col.colided) {
            for (auto enemy : enemyView)
            {
                if (col.colidedWith == enemy)
                    colidedWithEnemy = true;
            }

            if (Engine::Registry().valid(col.colidedWith) && !colidedWithEnemy)
            {
                SimpleCollision& collision = otherViews.get<SimpleCollision>(col.colidedWith);
                Transform& transform = otherViews.get<Transform>(col.colidedWith);
                Vector2 collisionSides = col.GetCollisionSides(t.position, collision, transform.position);


                do {
                    if (std::abs(collisionSides.x) > 0)
                        t.position.x -= collisionSides.x * Engine::DeltaTime();

                    if (std::abs(collisionSides.y) > 0)
                        t.position.y -= collisionSides.y * Engine::DeltaTime();
                } while (col.IsCollided(t.position, collision, transform.position));

                //t.position = { playerPos, 0.0 };
            }
        }

        colidedWithEnemy = false;
        col.colided = false;
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
            s.color.a = 1;
            //Missing: Activate sprite transparency or add sprite back
            r.position.x=t.position.x = playerPos.x + 8 * cos_;
            r.position.y=t.position.y = playerPos.y + 8 * sin_;
            dir.x = t.position.x - playerPos.x;
            dir.y = t.position.y - playerPos.y;
            s.color.r = r.isMouseBtnPressed ? 0 : 1;
        }
        else {
            s.color.a = 0;
            //Missing: Make sprite transparent or remove sprite.
        }
    }
    auto viewEnemy = Engine::Registry().view<EnemyData>();
    auto viewMelee = Engine::Registry().view<Transform, Sprite, MeleeWeaponSystem>();
    for (auto entity : viewMelee)
    {
        auto& t = viewMelee.get<Transform>(entity);
        auto& s = viewMelee.get<Sprite>(entity);
        auto& m = viewMelee.get<MeleeWeaponSystem>(entity);
        m.isActive = !focusRanged;
        if (m.isActive) {
            s.color.a = 1;
            //Missing: Activate sprite transparency or add sprite back
            t.position.x = playerPos.x + 8 * cos_;
            t.position.y = playerPos.y + 8 * sin_;
            s.color.r = m.isMouseBtnPressed ? 0 : 1;
        }
        else {
            s.color.a = 0;
            //Missing: Make sprite transparent or remove sprite.
        }
    }
    auto viewBullet = Engine::Registry().view<Transform, Sprite, BulletSystem, SimpleCollision>();
    for (auto entity : viewBullet)
    {
        auto& t = viewBullet.get<Transform>(entity);
        auto& s = viewBullet.get<Sprite>(entity);
        auto& b = viewBullet.get<BulletSystem>(entity);
        auto& col = viewBullet.get<SimpleCollision>(entity);
        //Privremeni TimeToLive sistem za metke.
        //if ((--b.ttl == 0) {
        if (col.colided) {
            if (Engine::Registry().valid(col.colidedWith)) {
                for (auto enemy : viewEnemy) {
                    if (col.colidedWith == enemy) {
                        int idx = b.index;
                        delete bullets[b.index];
                        bullets.erase(idx);
                        continue;
                    } //Trenutno brise metke samo pri sudaru sa enemy-jem. Kasnije dodati provere za zidove i ostale stvari.
                }
            }
        }
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
    for (auto entity : enemyView) 
    {
        auto& t = enemyView.get<Transform>(entity);
        auto& enemy = enemyView.get<EnemyData>(entity);
        auto& mov = enemyView.get<MovementData>(entity);
        auto& patrol = enemyView.get<Patrol>(entity);
        auto& col = enemyView.get<SimpleCollision>(entity);

        float distance = sqrt(pow(t.position.x - playerPosition.x, 2) + pow(t.position.y - playerPosition.y, 2));
        if (distance < enemy.visionDistance)
        {
            enemy.focusOnPlayer = true;
            enemy.visionDistance += 0.01;
        }
        else if (distance > 1.5 * enemy.visionDistance)
            enemy.focusOnPlayer = false;

        if (!enemy.focusOnPlayer)
        {
            Vector3 currentWaypoint = patrol.waypoints[patrol.currentWaypointIndex];

            Vector3 direction = currentWaypoint - t.position;
            float length = sqrt(direction.x * direction.x + direction.y * direction.y);
                
            if (length <= 1.0f)
            {
                patrol.forward = (rand() % 2) > 0.5f ? true : false;
                if (patrol.forward)
                {
                    patrol.currentWaypointIndex = (patrol.currentWaypointIndex + 1) % patrol.waypoints.size();
                }
                else
                {
                    patrol.currentWaypointIndex = patrol.currentWaypointIndex - 1;
                    if (patrol.currentWaypointIndex < 0)
                    {
                        patrol.currentWaypointIndex = patrol.waypoints.size() - 1;
                    }
                }
                t.position = currentWaypoint;
            }
            else
            {
                if (length != 0)
                {
                    direction /= length;
                }

                t.position.x += direction.x * mov.speed * Engine::DeltaTime();
                t.position.y += direction.y * mov.speed * Engine::DeltaTime();
            }
        }
        else
        {
            float directionX = playerPosition.x - t.position.x;
            float directionY = playerPosition.y - t.position.y;
            float length = sqrt(directionX * directionX + directionY * directionY);

            if (length != 0)
            {
                directionX /= length;
                directionY /= length;
            }

            mov.velocity.x = directionX * mov.speed * Engine::DeltaTime();
            mov.velocity.y = directionY * mov.speed * Engine::DeltaTime();

            t.position.x += mov.velocity.x;
            t.position.y += mov.velocity.y;
        }

        if (col.colided) {
            for (auto enemy : enemyView)
            {
                if (col.colidedWith == enemy)
                    colidedWithEnemy = true;
            }

            if (Engine::Registry().valid(col.colidedWith) && col.colidedWith != *view.begin() && !colidedWithEnemy)
            {
                SimpleCollision& collision = otherViews.get<SimpleCollision>(col.colidedWith);
                Transform& transform = otherViews.get<Transform>(col.colidedWith);
                Vector2 collisionSides = col.GetCollisionSides(t.position, collision, transform.position);

                do {
                    if (std::abs(collisionSides.x) > 0)
                        t.position.x -= collisionSides.x * Engine::DeltaTime();

                    if (std::abs(collisionSides.y) > 0)
                        t.position.y -= collisionSides.y * Engine::DeltaTime();
                } while (col.IsCollided(t.position, collision, transform.position));
            }
        }
        colidedWithEnemy = false;
        col.colided = false;
    }
}
