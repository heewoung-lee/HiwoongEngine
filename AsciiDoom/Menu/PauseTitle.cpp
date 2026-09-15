#include "PauseTitle.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/TransformComponent.h"
#include "Render/Renderer.h"

namespace Hiwoong
{
    PauseTitle::PauseTitle()
    {
        const auto size = Renderer::Get().GetScreenSize();

        const std::string rows[] =
        {
            "####   ###  #   #  #### ##### #### ",
            "#   # #   # #   # #     #     #   #",
            "####  ##### #   #  ###  ####  #   #",
            "#     #   # #   #     # #     #   #",
            "#     #   #  ###  ####  ##### #### "
        };

        std::string image;

        for (const auto& row : rows)
        {
            std::string expandedRow;

            // 가로 두 배 확대
            for (char character : row)
            {
                expandedRow += character;
                expandedRow += character;
            }

            if (!image.empty())
                image += '\n';

            // 세로 두 배 확대
            image += expandedRow + '\n' + expandedRow;
        }

        AddComponent<SpriteRendererComponent>(
            image,
            Color::Yellow,
            200
        );

        GetComponent<TransformComponent>()->SetLocalPosition(
            Vector3(size.x / 2 - 34, size.y / 2 - 15, 0)
        );
    }
}