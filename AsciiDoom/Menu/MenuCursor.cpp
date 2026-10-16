#include "MenuCursor.h"
#include "Component/SpriteRendererComponent.h"

namespace Hiwoong
{
    void MenuCursor::Start()
    {
        super::Start();

        AddComponent<SpriteRendererComponent>(
            SpriteFrame(
                {
                    "#  ",
                    " # ",
                    "  #",
                    " # ",
                    "#  "
                },
                {
                    "Y  ",
                    " Y ",
                    "  Y",
                    " Y ",
                    "Y  "
                }
            ),
            202
        );
    }

    void MenuCursor::SetTarget(
        const std::shared_ptr<GameObject>& target)
    {
        SetParent(target, false);
        SetPosition(Vector3(-6.0f, 0.0f, 0.0f));
    }
}