#include "game.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/audio.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite_render.h"
#include "core/graphics/tool_render.h"
#include "core/graphics/textures.h"
#include "core/graphics/animations.h"
#include "core/graphics/gui.h"
#include "tools/diagnostics.h"

void dagger::Game::CoreSystemsSetup()
{
    auto& engine = Engine::Instance();
    engine.AddSystem<AudioSystem>();
    engine.AddSystem<WindowSystem>();
    engine.AddSystem<InputSystem>();
    engine.AddSystem<ShaderSystem>();
    engine.AddSystem<TextureSystem>();
    engine.AddSystem<SpriteRenderSystem>();
    engine.AddSystem<AnimationSystem>();
    engine.AddSystem<TransformSystem>();
#if !defined(NDEBUG)
    engine.AddSystem<DiagnosticSystem>();
    engine.AddSystem<GUISystem>();
    engine.AddSystem<ToolMenuSystem>();
    engine.AddSystem<ToolRenderSystem>();
#endif //!defined(NDEBUG)
}