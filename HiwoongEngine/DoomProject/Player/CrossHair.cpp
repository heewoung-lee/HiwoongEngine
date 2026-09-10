#include "Component/SpriteRendererComponent.h"
#include "Component/TransformComponent.h"
#include "Render/Renderer.h"
#include "CrossHair.h"

namespace Hiwoong
{
	Crosshair::Crosshair()
	{
		AddComponent<SpriteRendererComponent>(
			"   |   \n"
			"-- 0 --\n"
			"   |   ", Color::Green, 101
		);

		//스크린 사이즈 받기
		const auto size = Renderer::Get().GetScreenSize();

		GetComponent<TransformComponent>()->SetLocalPosition(
			Vector3(static_cast<float>(size.x / 2 - 3), static_cast<float>(size.y / 2 - 1), 0)
		);
	}
}

