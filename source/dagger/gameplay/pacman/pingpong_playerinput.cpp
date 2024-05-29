#include "pingpong_playerinput.h"

#include "gameplay/common/simple_collisions.h"

#include "core/graphics/sprite.h"
#include "core/engine.h"
#include "core/game/transforms.h"
#include <imgui.h>
#include <tools/toolmenu.h>
#include<vector>

using namespace dagger;
using namespace pacman;

Float32 PingPongPlayerInputSystem::s_BoarderUp = 25;
Float32 PingPongPlayerInputSystem::s_BoarderDown = -25;
Float32 PingPongPlayerInputSystem::s_PlayerSpeed = 1.f;

bool left, right, up, down = false;
int mvR, mvL, mvU, mvB = 1;

char directions[] = {'U', 'D', 'L', 'R'};
  
int randIndex = rand() % 4;


void pacman::PingPongPlayerInputSystem::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&PingPongPlayerInputSystem::OnKeyboardEvent>(this);
    Engine::Dispatcher().sink<ToolMenuRender>().connect<&PingPongPlayerInputSystem::RenderToolMenu>(this);

    Engine::Dispatcher().sink<GUIRender>().connect<&PingPongPlayerInputSystem::RenderToolWindow>(this);
}

void pacman::PingPongPlayerInputSystem::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&PingPongPlayerInputSystem::OnKeyboardEvent>(this);
    Engine::Dispatcher().sink<ToolMenuRender>().disconnect<&PingPongPlayerInputSystem::RenderToolMenu>(this);

    Engine::Dispatcher().sink<GUIRender>().disconnect<&PingPongPlayerInputSystem::RenderToolWindow>(this);
}

void PingPongPlayerInputSystem::OnKeyboardEvent(KeyboardEvent kEvent_)
{
    Engine::Registry().view<ControllerMapping>().each([&](ControllerMapping& ctrl_)
    {
        //Y
        if (kEvent_.key == ctrl_.up_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
        {
            ctrl_.input.y = 1;
            up = true;
            left = false;
            right = false;
            down = false;
        }
        else if (kEvent_.key == ctrl_.up_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.y > 0)
        {
            ctrl_.input.y = 1;
        }
        else if (kEvent_.key == ctrl_.down_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Pressed))
        {
            ctrl_.input.y = -1;
            down = true;
            left = false;
            right = false;
            up = false;
        }
        else if (kEvent_.key == ctrl_.down_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.y < 0)
        {
            ctrl_.input.y = -1;
        }

        //X
        if (kEvent_.key == ctrl_.right_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
        {
            ctrl_.input.x = 1;
            right = true;
            left = false;
            down = false;
            up = false;
        }
        else if (kEvent_.key == ctrl_.right_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x > 0)
        {
            ctrl_.input.x = 1;
        }
        else if (kEvent_.key == ctrl_.left_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Pressed))
        {
            ctrl_.input.x = -1;

                left = true;
                right = false;
                down = false;
                up = false;
        }
        else if (kEvent_.key == ctrl_.left_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x < 0)
        {
            ctrl_.input.x = -1;
        }
    });
}


#if !defined(NDEBUG)
void PingPongPlayerInputSystem::RenderToolMenu()
{
    if (ImGui::BeginMenu("Nas test"))
    {
        if (ImGui::MenuItem("Pause"))
        {
            dagger::Engine::ToggleSystemsPause(true);
        }
        ImGui::EndMenu();
    }
}

void PingPongPlayerInputSystem::RenderToolWindow()
{
    ImGui::Begin("Nas prozor");

    int i = 0;

    auto view = Engine::Registry().view<Transform>();
    for (auto entity : view)
    {
        ImGui::PushID(i);

        auto& t = view.get<Transform>(entity);

        int pos[]{ t.position.x, t.position.y };
        ImGui::SliderInt2("Pixel Size", pos, -10000.0f, 10000.0f, "%f", 1);

        t.position.x = pos[0];
        t.position.y = pos[1];

        i++;
        ImGui::PopID();
    }

    ImGui::End();
}

#endif //!defined(NDEBUG)

void pacman::PingPongPlayerInputSystem::Run()
{
    Vector3 playerPosition{ 0, 0, 0 };

    //Pacman
    auto view = Engine::Registry().view<Transform, ControllerMapping, MovementData, SimpleCollision, Sprite>();
    for (auto entity : view)
    {

        auto& spr = view.get<Sprite>(entity);
        auto& t = view.get<Transform>(entity);
        auto& ctrl = view.get<ControllerMapping>(entity);
        auto& mov = view.get<MovementData>(entity);
        auto& col = view.get<SimpleCollision>(entity);
        auto viewCollisions = Engine::Registry().view<Transform, SimpleCollision>();

        //TODO LITTLE BUG FIX
        if (left) {
            AssignSprite(spr, "Pacmanart:pacmanleft:1");
            if (col.colided) {
                if (Engine::Registry().valid(col.colidedWith)) {
                    SimpleCollision& collision = viewCollisions.get<SimpleCollision>(col.colidedWith);
                    Transform& transform = viewCollisions.get<Transform>(col.colidedWith);
                    if (col.IsCollided(t.position, collision, transform.position)) {
                        t.position.x = transform.position.x + 20.0001;
                    }
                }
                col.colided = false;
                left = false;
            }
            else {
                t.position.x += ctrl.input.x;
            }
        }
        if (right) {
            AssignSprite(spr, "Pacmanart:pacmanright:1");
            if (col.colided) {
                if (Engine::Registry().valid(col.colidedWith)) {
                    SimpleCollision& collision = viewCollisions.get<SimpleCollision>(col.colidedWith);
                    Transform& transform = viewCollisions.get<Transform>(col.colidedWith);
                    if (col.IsCollided(t.position, collision, transform.position)) {
                        t.position.x = transform.position.x - 20.0001;
                    }
                }
                col.colided = false;
                right = false;
            }
            else
                t.position.x += ctrl.input.x;
        }



        if (up) {
            AssignSprite(spr, "Pacmanart:pacmanup:1");
            if (col.colided) {
                if (Engine::Registry().valid(col.colidedWith)) {
                    SimpleCollision& collision = viewCollisions.get<SimpleCollision>(col.colidedWith);
                    Transform& transform = viewCollisions.get<Transform>(col.colidedWith);
                    if (col.IsCollided(t.position, collision, transform.position)) {
                        t.position.y = transform.position.y - 20.0001;
                    }
                }
                col.colided = false;
                up = false;
            }
            else
                t.position.y += ctrl.input.y;
        }
        if (down) {
            AssignSprite(spr, "Pacmanart:pacmandown:1");
            if (col.colided) {
                if (Engine::Registry().valid(col.colidedWith)) {
                    SimpleCollision& collision = viewCollisions.get<SimpleCollision>(col.colidedWith);
                    Transform& transform = viewCollisions.get<Transform>(col.colidedWith);
                    if (col.IsCollided(t.position, collision, transform.position)) {
                        t.position.y = transform.position.y + 20.0001;
                    }
                }
                col.colided = false;
                down = false;
            }
            else
                t.position.y += ctrl.input.y;
        }

        playerPosition = t.position;
    }
    //Ghosts




    auto viewEnemy = Engine::Registry().view<EnemyData, Transform, MovementData, SimpleCollision>();
    for (auto entity : viewEnemy)
    {
        auto& t = viewEnemy.get<Transform>(entity);
        auto& mov = viewEnemy.get<MovementData>(entity);
        auto& enemy = viewEnemy.get<EnemyData>(entity);
        auto& col = viewEnemy.get<SimpleCollision>(entity);
        auto viewCollisions = Engine::Registry().view<Transform, SimpleCollision>();

        if (col.colided) {
            printf("Colided");
        }

        if (directions[randIndex] == 'U') {
            if (col.colided) {
                if (Engine::Registry().valid(col.colidedWith)) {
                    SimpleCollision& collision = viewCollisions.get<SimpleCollision>(col.colidedWith);
                    Transform& transform = viewCollisions.get<Transform>(col.colidedWith);
                    if (col.IsCollided(t.position, collision, transform.position)) {
                        t.position.y = transform.position.y - 20.0001;
                    }
                }
                col.colided = false;
                randIndex = rand() % 4;
            }
            else
                t.position.y += 1;
        }
        else if (directions[randIndex] == 'D') {
            if (col.colided) {
                if (Engine::Registry().valid(col.colidedWith)) {
                    SimpleCollision& collision = viewCollisions.get<SimpleCollision>(col.colidedWith);
                    Transform& transform = viewCollisions.get<Transform>(col.colidedWith);
                    if (col.IsCollided(t.position, collision, transform.position)) {
                        t.position.y = transform.position.y + 20.0001;
                    }
                }
                col.colided = false;
                randIndex = rand() % 4;
            }
            else
                t.position.y += -1;
        }
        else if (directions[randIndex] == 'L') {
            if (col.colided) {
                if (Engine::Registry().valid(col.colidedWith)) {
                    SimpleCollision& collision = viewCollisions.get<SimpleCollision>(col.colidedWith);
                    Transform& transform = viewCollisions.get<Transform>(col.colidedWith);
                    if (col.IsCollided(t.position, collision, transform.position)) {
                        t.position.x = transform.position.x + 20.0001;
                    }
                }
                col.colided = false;
                randIndex = rand() % 4;
            }
            else
                t.position.x += -1;
        }
        else if (directions[randIndex] == 'R') {
            if (col.colided) {
                if (Engine::Registry().valid(col.colidedWith)) {
                    SimpleCollision& collision = viewCollisions.get<SimpleCollision>(col.colidedWith);
                    Transform& transform = viewCollisions.get<Transform>(col.colidedWith);
                    if (col.IsCollided(t.position, collision, transform.position)) {
                        t.position.x = transform.position.x - 20.0001;
                    }
                }
                col.colided = false;
                randIndex = rand() % 4;
            }
            else
                t.position.x += 1;
        }
        //printf("%d", randIndex);
    }
}