#include "ResumeButton.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/TransformComponent.h"
#include "Render/Renderer.h"

namespace Hiwoong
{
    ResumeButton::ResumeButton()
    {
        AddComponent<SpriteRendererComponent>(
            "####  #####  #### #   # #   # #####\n"
            "#   # #     #     #   # ## ## #    \n"
            "####  ####   ###  #   # # # # #### \n"
            "#  #  #         # #   # #   # #    \n"
            "#   # ##### ####   ###  #   # #####",
            Color::Green,
            201
        );

        const auto size = Renderer::Get().GetScreenSize();

        GetComponent<TransformComponent>()->SetLocalPosition(
            Vector3(size.x / 2 - 17, size.y / 2, 0)
        );
    }
}