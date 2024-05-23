#include "PlayerController.h"
#include "Player.h"

#include "core/engine.h"
#include "core/game/transforms.h"

using namespace bober_game;

void PlayerController::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&PlayerController::OnKeyboardEvent>(this);
}

void PlayerController::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&PlayerController::OnKeyboardEvent>(this);
}

void PlayerController::OnKeyboardEvent(KeyboardEvent kEvent_)
{
    Engine::Registry().view<ControllerMapping>().each([&](ControllerMapping& ctrl_)
    {
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

void PlayerController::Run()
{
    Vector3 playerPosition;

    auto view = Engine::Registry().view<Transform, ControllerMapping, MovementData>();
    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& ctrl = view.get<ControllerMapping>(entity);
        auto& mov = view.get<MovementData>(entity);

        playerPosition = t.position;

        if (ctrl.input == Vector2{0.0f, 0.0f})
            break;

        double normalized = 1 / sqrt(ctrl.input.x * ctrl.input.x + ctrl.input.y * ctrl.input.y);

        t.position.x += normalized * ctrl.input.x * mov.speed * Engine::DeltaTime();
        t.position.y += normalized * ctrl.input.y * mov.speed * Engine::DeltaTime();

        playerPosition = t.position;

    }

    auto enemyView = Engine::Registry().view<Transform, EnemyData, MovementData, Patrol>();
    for (auto entity : enemyView) 
    {
        auto& t = enemyView.get<Transform>(entity);
        auto& enemy = enemyView.get<EnemyData>(entity);
        auto& mov = enemyView.get<MovementData>(entity);
        auto& patrol = enemyView.get<Patrol>(entity);

        float distance = sqrt(pow(t.position.x - playerPosition.x, 2) + pow(t.position.y - playerPosition.y, 2));
        if (distance < 50.f)
            enemy.focusOnPlayer = true;
        else if (distance > 150.f)
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
    }

 

    /*auto viewPosition = Engine::Registry().view<Transform, MovementData>();
    for (auto entity : viewPosition)
    {
        auto& t = viewPosition.get<Transform>(entity);
        auto& mov = viewPosition.get<MovementData>(entity);

        t.position.x += mov.velocity.x * Engine::DeltaTime();
        t.position.y += mov.velocity.y * Engine::DeltaTime();
    }*/
    
}
