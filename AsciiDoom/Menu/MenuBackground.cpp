#include "MenuBackground.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/TransformComponent.h"
#include "Render/Renderer.h"

namespace Hiwoong
{
    MenuBackground::MenuBackground()
    {
        const auto size = Renderer::Get().GetScreenSize();

        const int width = 80;
        const int height = 34;

        std::string image;

        for (int y = 0; y < height; ++y)
        {
            image += std::string(width, '#');

            if (y + 1 < height)
                image += '\n';
        }

        // 검정 문자로 뒤에 있는 게임 화면을 덮는다.
        AddComponent<SpriteRendererComponent>(
            image,
            Color::Black,
            199
        );

        GetComponent<TransformComponent>()->SetLocalPosition(
            Vector3(
                size.x / 2 - width / 2,
                size.y / 2 - height / 2,
                0
            )
        );
    }
}