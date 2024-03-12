#pragma once

#include "core/core.h"
#include "core/system.h"
#include <functional>

#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/game/transforms.h"

template<typename T>
JSON::json SerializeComponent(T& input_)
{
    assert(("Serialization failed: no type specialization found!", false));
    return JSON::json{};
}

template<typename T>
void DeserializeComponent(JSON::json input_, T& fill_)
{
    assert(("Deserialization failed: no type specialization found!", false));
}

// Serialize vectors

template<>
JSON::json SerializeComponent(Vector2& input_)
{
    return JSON::json{ { "x", input_.x }, { "y", input_.y } };
}

template<>
void DeserializeComponent(JSON::json input_, Vector2& fill_)
{
    fill_.x = input_["x"];
    fill_.y = input_["y"];
}

template<>
JSON::json SerializeComponent(Vector3& input_)
{
    return JSON::json{ { "x", input_.x }, { "y", input_.y }, { "z", input_.z } };
}

template<>
void DeserializeComponent(JSON::json input_, Vector3& fill_)
{
    fill_.x = input_["x"];
    fill_.y = input_["y"];
    fill_.z = input_["z"];
}

template<>
JSON::json SerializeComponent(Vector4& input_)
{
    return JSON::json{ { "x", input_.x }, { "y", input_.y }, { "z", input_.z }, { "w", input_.w } };
}

template<>
void DeserializeComponent(JSON::json input_, Vector4& fill_)
{
    fill_.x = input_["x"];
    fill_.y = input_["y"];
    fill_.z = input_["z"];
    fill_.w = input_["w"];
}

// Serialize sprites

template<>
JSON::json SerializeComponent(Sprite& input_)
{
    JSON::json save{};
    save["subSize"] = SerializeComponent(input_.subSize);
    save["subOrigin"] = SerializeComponent(input_.subOrigin);
    save["size"] = SerializeComponent(input_.size);
    save["position"] = SerializeComponent(input_.position);
    save["pivot"] = SerializeComponent(input_.pivot);
    save["color"] = SerializeComponent(input_.color);
    save["scale"] = SerializeComponent(input_.scale);
    save["rotation"] = input_.rotation;
    save["isUI"] = input_.isUI;
    save["image"] = input_.image->Name().c_str();
    save["shader"] = input_.shader->shaderName.c_str();
    return save;
}

template<>
void DeserializeComponent(JSON::json input_, Sprite& fill_)
{
    AssignSpriteShader(fill_, input_["shader"]);
    AssignSprite(fill_, input_["image"]);
    DeserializeComponent(input_["subSize"], fill_.subSize);
    DeserializeComponent(input_["subOrigin"], fill_.subOrigin);
    DeserializeComponent(input_["size"], fill_.size);
    DeserializeComponent(input_["position"], fill_.position);
    DeserializeComponent(input_["pivot"], fill_.pivot);
    DeserializeComponent(input_["color"], fill_.color);
    DeserializeComponent(input_["scale"], fill_.scale);
    fill_.rotation = input_["rotation"];
    fill_.isUI = input_["isUI"];
}

// Serialize transforms

template<>
JSON::json SerializeComponent(Transform& input_)
{
    JSON::json save{};
    save["position"] = SerializeComponent(input_.position);
    return save;
}

template<>
void DeserializeComponent(JSON::json input_, Transform& fill_)
{
    DeserializeComponent(input_["position"], fill_.position);
}

// Serialize animations

template<>
JSON::json SerializeComponent(Animator& input_)
{
    JSON::json save{};
    save["name"] = input_.currentAnimation.c_str();
    return save;
}

template<>
void DeserializeComponent(JSON::json input_, Animator& fill_)
{
    AnimatorPlay(fill_, input_["name"]);
}

template<>
JSON::json SerializeComponent(SimpleCollision& collision_)
{
    JSON::json save{};
    save["size"] = SerializeComponent(collision_.size);
    save["pivot"] = SerializeComponent(collision_.pivot);
    return save;
}

template<>
void DeserializeComponent(JSON::json input_, SimpleCollision& fill_)
{
    DeserializeComponent(input_["size"], fill_.size);
    DeserializeComponent(input_["pivot"], fill_.pivot);
}
