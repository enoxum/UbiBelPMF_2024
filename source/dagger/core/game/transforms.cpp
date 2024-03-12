#include "transforms.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"

#include <algorithm>
#include <execution>

using namespace dagger;

void TransformSystem::Run()
{
    Engine::Registry().view<Transform, Sprite>().each([](const Transform& transform_, Sprite& sprite_)
        {
            sprite_.position = transform_.position;
        });
}