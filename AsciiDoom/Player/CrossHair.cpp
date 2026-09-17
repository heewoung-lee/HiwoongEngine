#include "Component/SpriteRendererComponent.h"
#include "Component/TransformComponent.h"
#include "Component/CrossHairMoveComponent.h"
#include "Render/Renderer.h"
#include "CrossHair.h"
#include "Scene/DoomScene.h"
#include <cassert>


namespace Hiwoong
{
	Crosshair::Crosshair()
	{
		AddComponent<SpriteRendererComponent>(
			std::vector<std::string>{
			"   |   ",
				"-- 0 --",
				"   |   "
		},
			Color::Green,
			101
		);
	}

	void Crosshair::Start()
	{
		super::Start();

		const auto scene =
			std::dynamic_pointer_cast<DoomScene>(GetOwner());
		assert(scene != nullptr);

		const Vector2 size = scene->GetGameSize();

		GetComponent<TransformComponent>()->SetLocalPosition(
			Vector3(size.x / 2 - 3, size.y / 2 - 1, 0)
		);

		//크로스헤어의 Y축을 움직일 컴포넌트 
		AddComponent<CrossHairMoveComponent>(30.0f, 50.0f);


	}
	Vector2 Crosshair::GetScreenCenter() const
	{
		// 현재 그림은 가로 7칸, 세로 3칸.
		// 그림의 시작 위치에서 가운데 '0'까지의 거리.
		constexpr float centerOffsetX = 3.0f;
		constexpr float centerOffsetY = 1.0f;

		// SpriteRenderer가 실제로 그리는 위치를 기준으로 한다.
		const Vector3 position = GetWorldPosition();

		return Vector2(
			position.x + centerOffsetX,
			position.y + centerOffsetY
		);
	}
}

