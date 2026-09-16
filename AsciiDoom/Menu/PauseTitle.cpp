#include "PauseTitle.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/TransformComponent.h"
#include "Render/Renderer.h"
#include <vector>
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

        std::vector<std::string> image;

        for (const auto& row : rows)
        {
            std::string expandedRow;

            for (char character : row)
            {
                expandedRow += character;
                expandedRow += character;
            }

            image.emplace_back(expandedRow);
            image.emplace_back(expandedRow);
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