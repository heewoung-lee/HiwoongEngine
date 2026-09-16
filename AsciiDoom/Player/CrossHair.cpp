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
}

