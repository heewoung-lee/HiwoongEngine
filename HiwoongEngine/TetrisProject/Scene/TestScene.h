#pragma once

#include "Scene/Scene.h"
#include "Math/Vector2.h"

namespace Hiwoong
{
	class TestScene : public Hiwoong::Scene
	{
	public:

		TestScene();
		~TestScene();

		virtual void SceneInitialize() override;

	};
}
