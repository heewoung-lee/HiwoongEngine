#include "TestScene.h"
#include "GameObject/TestGameObject.h"
#include "Math/Vector2.h"
#include "Scene/Scene.h"
#include "GameObject/BackGround/Wall.h"
#include "GameObject/BackGround/BackGround.h"
#include "GameObject/SpawnManager/SpawnManager.h"
#include "Scene/GameOver.h"
#include "Engine/Engine.h"
#include "Util/FileUtil.h"
#include "Manager/TetrisGameState.h"
#include "Scene/GameStatesUI.h"

#include <fstream>
#include <cassert>
#include <memory>

#include <iostream>
#include <algorithm>

namespace Hiwoong
{

	TestScene::TestScene() : Scene()
	{
		const int currentLevel =
			TetrisGameState::Get().GetCurrentLevel();
	}
	TestScene::~TestScene()
	{
	}
	double TestScene::CalculateDropInterval() const
	{
		const int currentLevel =
			TetrisGameState::Get().GetCurrentLevel();

		return (std::max)(
			0.1,
			1.0 - (currentLevel - 1) * 0.1
			);
	}

	void TestScene::SceneInitialize()
	{

		//Load Map
		Vector2 screenSize = LoadMap("map.txt");

		const int playWidth = screenSize.x - 2;
		const int spwanX = 1 + (playWidth - 4) / 2;

		spawnManager = Instantiate<SpawnManager>(Vector2(spwanX,1));
		
		const int uiGap = 2;

		//expend ScreenSize
		const Vector2 totalScreenSize(
			screenSize.x + GameStatesUI::Width + uiGap,
			screenSize.y
		);

		Instantiate<GameStatesUI>(Vector2(screenSize.x + uiGap, 2));


		Scene::SetScreenSize(totalScreenSize);
		Scene::SceneInitialize();
		
	}

	void TestScene::Update(double deltaTime)
	{
		Scene::Update(deltaTime);

		std::shared_ptr<SpawnManager> manager = spawnManager.lock();

		assert(manager != nullptr);

		if (manager->IsGameOver() && hasStartedGameOver == false)
		{
			hasStartedGameOver = true;
			Engine::Get().AddNewScene<GameOver>();
			return;
		}

		//Check Player Score
		TetrisGameState& state = TetrisGameState::Get();

		if (state.GetScore() >= state.GetRequireNextLevelScore())
		{
			state.LevelUp();
			Engine::Get().AddNewScene<TestScene>();
		}

	}


	Vector2 TestScene::LoadMap(const std::string& filename)
	{
		const std::vector<std::string> lines = FileUtil::LoadTextLines("../Assets/Stages/" + filename);
		
		std::vector<Vector2> wallPositions;

		int mapWidth = 0;
		const int mapHeight = static_cast<int>(lines.size());

		for (int y = 0; y < mapHeight;++y)
		{
			const std::string& line = lines[y];

			//Find maxLength
			mapWidth = (std::max)(mapWidth, static_cast<int>(line.size()));
			for (int x = 0; x < line.size(); ++x)
			{
				const Vector2 position(x, y);

				switch (line[x])
				{
				case '#':
					Instantiate<Wall>(position);
					wallPositions.push_back(position);
					break;

				case '.':
					Instantiate<BackGround>(position);
					break;
				}
			}
		}
		board = std::make_unique<TetrisBoard>(mapWidth, mapHeight);
		 for (const Vector2& wallPosition : wallPositions)
    {
        board->SetWall(wallPosition);
    }

    return Vector2(mapWidth, mapHeight);
	}
}

