#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteRendererComponent.h"
namespace Hiwoong
{
    class TextObject : public GameObject
    {
    public:
        TextObject(
            const Vector2& position,
            const std::string& text,
            const Color& color)
            : GameObject(position)
        {
            AddComponent<SpriteRendererComponent>(
                text,
                color,
                10
            );
        }
    };
}
