#include "RestartButton.h"
#include "Component/SpriteRendererComponent.h"
#include "UI/BigTextDisplayComponent.h"
#include "Scene/Scene.h"

namespace Hiwoong
{
    void RestartButton::Start()
    {
        super::Start();

        AddComponent<SpriteRendererComponent>(
            SpriteFrame({}, {}),
            201
        );

        const auto text = AddComponent<BigTextDisplayComponent>(
            "RESTART", 1, 'S'
        );

        const auto screenSize = GetOwner()->GetScreenSize();
        const auto textSize = text->GetTextSize();

        const int x = (screenSize.x - textSize.x) / 2;
        const int y = screenSize.y / 2;

        SetPosition(Vector3(
            static_cast<float>(x),
            static_cast<float>(y),
            0.0f
        ));
    }
}