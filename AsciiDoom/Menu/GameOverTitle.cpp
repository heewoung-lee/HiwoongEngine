#include "GameOverTitle.h"
#include "Component/SpriteRendererComponent.h"
#include "UI/BigTextDisplayComponent.h"
#include "Scene/Scene.h"

namespace Hiwoong
{
    void GameOverTitle::Start()
    {
        super::Start();

        AddComponent<SpriteRendererComponent>(
            SpriteFrame({}, {}),
            200
        );

        const auto text = AddComponent<BigTextDisplayComponent>(
            "GAME OVER", 2, 'R'
        );

        const auto screenSize = GetOwner()->GetScreenSize();
        const auto textSize = text->GetTextSize();

        const int x = (screenSize.x - textSize.x) / 2;
        const int y = screenSize.y / 3 - textSize.y / 2;

        SetPosition(Vector3(
            static_cast<float>(x),
            static_cast<float>(y),
            0.0f
        ));
    }
}