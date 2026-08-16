#include "Manager/TetrisGameState.h"

namespace Hiwoong
{
	TetrisGameState& TetrisGameState::Get()
	{
		static TetrisGameState instance;
		return instance;
	}
	void TetrisGameState::AddScore(int score)
	{
		currentScore += score;
	}
	void TetrisGameState::LevelUp()
	{
		++currentLevel;
		requireNextLevel += 200;
	}
	void TetrisGameState::SetNextModuleNumber(int number)
	{
		nextModuleNumber = number;
	}
	void TetrisGameState::Reset()
	{
		currentScore = 0;
		currentLevel = 1;
		requireNextLevel = 100;
		nextModuleNumber = -1;
	}
}
