#include "LineTestObject.h"
#include "Render/Renderer.h"

namespace Hiwoong
{
    void LineTestObject::Draw()
    {
        GameObject::Draw();

        Renderer::Get().SubmitLine(
            Vector2(2, 2),
            Vector2(20, 10),
            '@',
            Color::White,
            0
        );
    }
}