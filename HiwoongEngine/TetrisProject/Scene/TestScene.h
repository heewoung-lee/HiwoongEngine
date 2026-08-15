#pragma once

#include "Scene/Scene.h"
#include "Math/Vector2.h"
#include "GameObject/BackGround/TetrisBoard.h"

namespace Hiwoong
{
	class SpawnManager;


	class TestScene : public Hiwoong::Scene
	{
	public:

		TestScene(int level = 1);
		~TestScene();

		virtual void SceneInitialize() override;
		virtual void Update(double deltatime) override;


		Vector2 LoadMap(const std::string& mapPath);
		double GetDropInterval() const
		{
			return CalculateDropInterval();
		}

		TetrisBoard* GetBoard() const
		{
			return board.get();
		}

	private:
		double CalculateDropInterval() const;

		int currentLevel = 1;
		std::unique_ptr<TetrisBoard> board;
		std::weak_ptr<SpawnManager> spawnManager;

		bool hasStartedGameOver = false;


	};
}
