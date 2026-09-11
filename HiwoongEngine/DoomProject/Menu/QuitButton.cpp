#include "QuitButton.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/TransformComponent.h"
#include "Render/Renderer.h"

namespace Hiwoong
{
    QuitButton::QuitButton()
    {
        AddComponent<SpriteRendererComponent>(
            " ###  #   # ##### #####\n"
            "#   # #   #   #     #  \n"
            "#   # #   #   #     #  \n"
            "#  ## #   #   #     #  \n"
            " ####  ###  #####   #  ",
            Color::Gray,
            201
        );

        const auto size = Renderer::Get().GetScreenSize();

        GetComponent<TransformComponent>()->SetLocalPosition(
            Vector3(size.x / 2 - 11, size.y / 2 + 8, 0)
        );
    }
}