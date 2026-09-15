#pragma once

#include "Scene/Scene.h"


namespace Hiwoong
{
	class GameOver : public Scene
	{
	public:

		GameOver() = default;
		~GameOver() override = default;

		
		void SceneInitialize() override;
		void Update(double deltatime) override;


	private:
		double elapsedTime = 0.0;
		bool showMessage = true;
	};
}
