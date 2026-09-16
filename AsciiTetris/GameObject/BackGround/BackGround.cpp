#include "BackGround.h"
#include "Component/SpriteRendererComponent.h"
#include "Math/Vector2.h"
#include "Math/Color.h"
#include <string>
#include <vector>

namespace Hiwoong
{
    BackGround::BackGround(const Vector2& position)
        : GameObject(Vector3(position.x, position.y, 0))
    {
        AddComponent<SpriteRendererComponent>(
            std::vector<std::string>{"."},
            Color::White,
            0
        );
    }
}