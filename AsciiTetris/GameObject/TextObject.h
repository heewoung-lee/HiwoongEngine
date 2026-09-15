#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteRendererComponent.h"
#include "Math/Vector2.h"
#include <memory>
#include <cassert>

namespace Hiwoong
{
    class TextObject : public GameObject
    {
    public:
        TextObject(
            const Vector2& position,
            const std::string& text,
            const Color& color)
            : GameObject(Vector3(position.x,position.y,0))
        {
            spriteRenderer = AddComponent<SpriteRendererComponent>(
                text,
                color,
                10
            );
        }


        void SetText(const std::string& text)
        {
            std::shared_ptr<SpriteRendererComponent> renderer = spriteRenderer.lock();
            assert(renderer != nullptr);
            renderer->SetImage(text);
        }

        void SetColor(Color color)
        {
            std::shared_ptr<SpriteRendererComponent> renderer =
                spriteRenderer.lock();

            assert(renderer != nullptr);

            renderer->SetColor(color);
        }
    
    private:
        std::weak_ptr<SpriteRendererComponent> spriteRenderer;
    };

}
