#include "QuitButton.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/TransformComponent.h"
#include "Render/Renderer.h"

namespace Hiwoong
{
    QuitButton::QuitButton()
    {
        const std::vector<std::string> image =
        {
            " ###  #   # ##### #####",
            "#   # #   #   #     #  ",
            "#   # #   #   #     #  ",
            "#  ## #   #   #     #  ",
            " ####  ###  #####   #  "
        };

        std::vector<std::string> colorRows;

        for (const std::string& row : image)
        {
            colorRows.emplace_back(row.size(), 'S');
        }

        AddComponent<SpriteRendererComponent>(
            SpriteFrame(image, colorRows),
            201
        );

        const auto size = Renderer::Get().GetScreenSize();

        GetComponent<TransformComponent>()->SetLocalPosition(
            Vector3(size.x / 2 - 11, size.y / 2 + 8, 0)
        );
    }
}