#pragma once

namespace Hiwoong
{
	class TetrisGameState
	{

	public:
		static TetrisGameState& Get();

		inline int GetScore() const { return currentScore; }
		inline int GetRequireNextLevelScore() const { return requireNextLevel; }
		inline int GetCurrentLevel() const { return currentLevel; }
		inline int GetNextModuleNumber() const { return nextModuleNumber; }

		void AddScore(int score);
		void LevelUp();
		void SetNextModuleNumber(int number);
		void Reset();

	private:
		TetrisGameState() = default;
		int currentScore = 0;
		int requireNextLevel = 100;
		int currentLevel = 100;
		int nextModuleNumber = -1;
	};

}


