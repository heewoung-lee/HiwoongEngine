#include "Util/FileUtil.h"
#include "GameOver.h"
#include "GameObject/TextObject.h"
#include "Math/Color.h"
#include "Core/Input.h"
#include "Engine/Engine.h"
#include "Scene/TestScene.h"
#include <Windows.h>


namespace Hiwoong
{
	void GameOver::SceneInitialize()
	{

		const auto lines = FileUtil::LoadTextLines(
			"../Assets/Stages/GameOver.txt"
		);

		int width = 0;

		for (const std::string& line : lines)
		{
			const int lineWidth = static_cast<int>(line.size());

			if (lineWidth > width)
			{
				width = lineWidth;
			}
		}

		const int height = static_cast<int>(lines.size());

		SetScreenSize(Vector2(width, height));

		for (int y = 0; y < lines.size(); ++y)
		{
			Instantiate<TextObject>(
				Vector2(0, y),
				lines[y],
				Color::Red
			);
		}
		Scene::SceneInitialize();
		

	}
	void GameOver::Update(double deltatime)
	{
		Scene::Update(deltatime);

		if (Input::Get().GetKeyDown('R'))
		{
			Engine::Get().AddNewScene<TestScene>();
			return;
		}

		if (Input::Get().GetKeyDown(VK_ESCAPE))
		{
			//TODO: Initialze Singleton
			Engine::Get().Quit();
		}

	}
}