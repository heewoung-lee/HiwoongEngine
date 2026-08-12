#pragma once

#include "Scene/Scene.h"
#include "Math/Vector2.h"
namespace Hiwoong
{
	class TestScene : public Hiwoong::Scene
	{
	public:

		TestScene(int level = 1);
		~TestScene();

		virtual void SceneInitialize() override;
		

		Vector2 LoadMap(const std::string& mapPath);
		double GetDropInterval() const
		{
			return CalculateDropInterval();
		}

	private:
		double CalculateDropInterval() const;
		
		int currentLevel = 1;

	};
}
