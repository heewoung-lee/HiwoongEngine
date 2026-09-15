#include "MenuFrame.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/TransformComponent.h"
#include "Render/Renderer.h"

namespace Hiwoong
{
    MenuFrame::MenuFrame()
    {
        const auto size = Renderer::Get().GetScreenSize();

        const int width = 80;
        const int height = 34;

        std::string image;

        for (int y = 0; y < height; ++y)
        {
            // 위·아래 테두리와 제목 아래 구분선
            if (y == 0 || y == height - 1 || y == 13)
            {
                image += "+" + std::string(width - 2, '-') + "+";
            }
            else
            {
                image += "|" + std::string(width - 2, ' ') + "|";
            }

            if (y + 1 < height)
                image += '\n';
        }

        AddComponent<SpriteRendererComponent>(
            image,
            Color::Yellow,
            200
        );

        // 검정 배경과 같은 위치에 배치.
        GetComponent<TransformComponent>()->SetLocalPosition(
            Vector3(
                size.x / 2 - width / 2,
                size.y / 2 - height / 2,
                0
            )
        );
    }
}