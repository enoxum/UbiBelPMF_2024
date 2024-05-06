#include "bullethell_bar.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"
#include "core/game/transforms.h"

using namespace ping_pong;

void BarSystem::Run() {
    auto view = Engine::Registry().view<Bar>();
    for(auto& bar : view){
        auto& barData = Engine::Registry().get<Bar>(bar);
        if(*barData.tracking != barData.amount){
//            Logger::info("{} {}", barData.amount, *barData.tracking);
            barData.amount = *barData.tracking;
            float ratio = (float)glm::clamp(barData.amount, 0.f, barData.max) / (float)barData.max;
            auto innerSprite = Engine::Registry().try_get<Sprite>(barData.inner);
            if(innerSprite){
//                Logger::info(ratio);
                innerSprite->scale = {ratio, 1};
//                innerSprite->pivot = {1.f , 0};
                innerSprite->position.x = barData.position.x - (barData.width - innerSprite->size.x * innerSprite->scale.x) / 2.f;
//                Logger::info(innerSprite->position.x);
            }
        }
    }
}

Entity ping_pong::createBar(float *toTrack, Vector3 position, int width, int height, Vector4 color, int max) {
    auto& reg = Engine::Registry();
    auto entity = reg.create();
    auto& transform = reg.emplace<Transform>(entity);
    transform.position = position;

    auto& bar = reg.emplace<Bar>(entity);

    auto outer = reg.create();
    auto inner = reg.create();


    auto& outerSprite = reg.emplace<Sprite>(outer);
    outerSprite.color = {0,0,0,1};
    AssignSprite(outerSprite, "EmptyWhitePixel");
    outerSprite.size = {width + 10, height + 10};
    outerSprite.position = transform.position;
    outerSprite.position.z = 1;


    auto& innerSprite = reg.emplace<Sprite>(inner);
    innerSprite.color = color;
    AssignSprite(innerSprite, "EmptyWhitePixel");
    innerSprite.size = {width, height};
    innerSprite.position = transform.position;
    innerSprite.position.z = 0;

    bar.inner = inner;
    bar.outer = outer;
    bar.amount = max;
    bar.max = max;
    bar.width = width;
    bar.height = height;
    bar.position = position;
    bar.tracking = toTrack;

    return entity;
}

